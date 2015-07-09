#include <stdio.h>
#include <string>
#include <future>
#include <unistd.h>
#include <exception>
#include <sys/socket.h>

#include <event2/util.h>
#include <arpa/inet.h>
#include <event2/event.h>
#include <event2/event_struct.h>
#include <assert.h>
#include <string.h>
#include "../libframework/ztimer.h"
#include "../libframework/zerrno.h"
#include "stream.h"


#define log(_format_, _args_...) printf(_format_ "\n", ##_args_)

namespace z {


class EventCallback {
public:
  virtual void onRead(evutil_socket_t fd) = 0;
  virtual void onWrite(evutil_socket_t fd) = 0;
  //virtual void onTimeout() = 0;
};


class Handler: public EventCallback, public z::Timer::TimerCallback {
public:
  Handler(struct event_base *base,
          evutil_socket_t fd,
          const std::string server_host, unsigned short server_port,
          const std::string client_host, unsigned short client_port,
          Stream *stream)
      : base_(base)
      , fd_(fd)
      , timer_(base, this)
      , server_host_(server_host), server_port_(server_port)
      , client_host_(client_host), client_port_(client_port)
      , stream_(stream)
  {
    assert(fd >= 0);
    assert(server_host.length() > 0);
    assert(client_host.length() > 0);
  }

public:
  void setEvent(struct event *ev) {
    assert(ev != nullptr);
    ev_ = ev;
  }
  evutil_socket_t getFd() { return fd_; }

  const std::string& getServerHost() { return server_host_; }

  unsigned short getServerPort() { return server_port_; }

  const std::string& getClientHost() { return client_host_; }

  unsigned short getClientPort() { return client_port_; }

  // from `EventCallback`
  virtual void onRead(evutil_socket_t fd) override {
    log("onRead(%d)", fd);
    // int rv = (int) ::read(fd, buf_, sizeof(buf_));
    int rv = stream_->read(buf_, sizeof(buf_));
    if (0 == rv) {
      log("peer closed");
      close();
      return;
    } else if (rv < 0) {
      perror("read");
      close();
      return;
    }

    onRead(fd, buf_, rv);
  }

  virtual void onWrite(evutil_socket_t fd) override {
    log("onWrite(%d)", fd);
  }

  // from Timer::TimerCallback
  virtual void onTimeout(int id) override {
    log("onTimeout(%d)", id);
  }

  // for child classes
  virtual void onRead(evutil_socket_t fd, char *buf, int buf_size) = 0;

protected:

  virtual void close() {
    assert(ev_ != nullptr);
    if (ev_ != nullptr) {
      event_free(ev_);
    }
    ::close(fd_);
  }

private:
  struct event_base *base_;
  const evutil_socket_t fd_;
  struct event *ev_;
  const std::string server_host_;
  const unsigned short server_port_;
  const std::string client_host_;
  const unsigned short client_port_;
  char buf_[10 << 10];
  z::Timer timer_;
  Stream *stream_;
};

class EchoHandler: public Handler {
public:
  EchoHandler(struct event_base *base,
              evutil_socket_t fd,
              const std::string server_host, unsigned short server_port,
              const std::string client_host, unsigned short client_port)
      : Handler(base, fd, server_host, server_port, client_host, client_port,
                new LayeredStream(fd, new TransparentLayer(nullptr)))
  {}

  typedef Handler super_;

public:
  virtual void onRead(evutil_socket_t fd, char *buf, int buf_len) override {
    std::string s(buf, buf_len);
    log("read: (%d, [%s]", buf_len, s.c_str());
    ::write(fd, buf, buf_len);
  }

private:

};

// TODO: to be deleted
class FixedHeaderHandler: public Handler {
public:
  FixedHeaderHandler(struct event_base *base,
                     evutil_socket_t fd,
                     const std::string server_host, unsigned short server_port,
                     const std::string client_host, unsigned short client_port)
      : Handler(base, fd, server_host, server_port, client_host, client_port,
                new LayeredStream(fd, new TransparentLayer(nullptr)))
  {}

public:
  virtual void onRead(evutil_socket_t fd, char *buf, int buf_len) override {
    std::string s(buf, buf_len);
    log("read: (%d, [%s]", buf_len, s.c_str());
    ::write(fd, buf, buf_len);
  }

  virtual void onWrite(evutil_socket_t fd) override {
    log("onWrite(%d)", fd);
  }

};

static void event_callback(evutil_socket_t fd, short events, void* arg) {
  assert(arg != nullptr);
  assert(!(events & EV_TIMEOUT));

  EventCallback *callback = (EventCallback *) arg;

  if (events & EV_READ) {
    callback->onRead(fd);
  }

  if (events & EV_WRITE) {
    callback->onWrite(fd);
  }
}

/**
 * move this out of the `Server` class to reduce code bloat of template.
 */
static evutil_socket_t listen(const std::string &host, unsigned short port)
{
  int rv;

  evutil_socket_t fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("socket");
    return -1;
  }

  evutil_make_socket_nonblocking(fd);

#ifndef WIN32
  {
    int one = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
  }
#endif // WIN32

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  inet_aton(host.c_str(), &sin.sin_addr);
  sin.sin_port = htons(port);

  rv = bind(fd, (struct sockaddr*) (&sin), sizeof(sin));
  if (rv < 0) {
    perror("bind");
    return -1;
  }

  rv = ::listen(fd, 16);
  if (rv < 0) {
    perror("listen");
    return -1;
  }

  return fd;
}

/**
 * move this out of the `Server` class to reduce code bloat of template.
 */
static evutil_socket_t accept(evutil_socket_t fd, std::string &host, unsigned short &port)
{
  struct sockaddr_storage ss;
  socklen_t slen = sizeof(ss);
  int cli_fd = ::accept(fd, (struct sockaddr*) (&ss), &slen);
  if (cli_fd < 0) {           // XXX EAGAIN?
    perror("accept");
    ::evutil_closesocket(cli_fd);
    return -1;
  }

  //
  struct sockaddr_in* addr = (struct sockaddr_in*)(&ss);

  // assign output parameters
  // FIXME: release the allocated memory in a multi-thread program
  // TODO: DIY it
  host.assign(inet_ntoa(addr->sin_addr));
  port = ntohs(addr->sin_port);

  evutil_make_socket_nonblocking(cli_fd);

  return cli_fd;
}


template <typename CHILD_HANDLER_T>
class Server: public EventCallback, public z::Timer::TimerCallback {
public:
  Server(struct event_base *base)
      : host_("0.0.0.0"), port_(0)
      , base_(base)
      , timer_(base, this)
  { }

  enum {
    BACKLOG,
    KEEPALIVE,
  };

public:
  Server& setHost(const std::string &host) {
    host_.assign(host);
    return *this;
  }

  Server& setPort(unsigned short port) {
    port_ = port;
    return *this;
  }

  Server& setOption(uint32_t opt, int value) {
    return *this;
  }

  Server& listen() {

    evutil_socket_t fd = z::listen(host_, port_);
    if (fd < 0) {
      // TODO
      throw std::exception();
    }

    // FIXME: when will delete it? check if `libevent` will release this object or not.
    struct event *ev = event_new(base_, fd, EV_READ|EV_PERSIST, event_callback, this);
    if (ev < 0) {
      throw std::exception();
    }

    event_add(ev, nullptr);

    return *this;
  }

  virtual void onTimeout(int id) override {
    log("onTimeout(%d)", id);
  }

protected:
  virtual void onRead(evutil_socket_t fd) override {

    std::string client_host;
    unsigned short client_port;
    evutil_socket_t client_fd = z::accept(fd, client_host, client_port);
    if (client_fd < 0) {
      // failed
      return;
    }

    log("accepted connection from: %s:%u", client_host.c_str(), client_port);

    CHILD_HANDLER_T *child_handler = new CHILD_HANDLER_T(
        base_, client_fd, host_, port_, client_host, client_port);
    struct event *ev = event_new(
        base_, client_fd, EV_READ|EV_PERSIST, event_callback, child_handler);
    child_handler->setEvent(ev);

    event_add(ev, nullptr);

  }

  virtual void onWrite(evutil_socket_t fd) override {
    log("onWrite(%d)", fd);
  }

private:
  std::string host_;
  unsigned short port_;
  struct event_base *base_;
  z::Timer timer_;
};


int connect(std::string host, unsigned short port, evutil_socket_t &fd)
{
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    log("failed to get new socket file descriptor");
    return FAIL;
  }

  evutil_make_socket_nonblocking(fd);

#ifndef WIN32
  {
    int one = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one))) {
      perror("setsockopt");
      ::close(fd);
      return FAIL;
    }
  }
#endif // WIN32

  int rv;

  // // need bind or not?
  // if (false) {
  //  rv = bind(fd, (struct sockaddr*) (&sin), sizeof(sin));
  //  if (rv < 0) {
  //    perror("bind");
  //    return false;
  //  }
  // }

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(host.c_str());
  sin.sin_port = htons(port);

  rv = ::connect(fd, (struct sockaddr*) (&sin), sizeof(sin));
  if (rv < 0) {
    if (errno != EINPROGRESS) {

      perror("connect");
      log("Can not initial connection");
      // ::close(fd);
      evutil_closesocket(fd);
      return FAIL;
    } else {
      return ERR_IO_PENDING;
    }
  }

  return OK;
}

class Client: public EventCallback, public z::Timer::TimerCallback {
public:
  Client(struct event_base *base, const std::string &server_host, unsigned short server_port)
      : base_(base)
      , server_host_(server_host)
      , server_port_(server_port)
      , fd_(-1)
      , ev_(nullptr)
      , state_(DISCONNECTED)
      , timer_(base, this)
  {}

  typedef enum {
    DISCONNECTED,
    WAIT_FOR_RSP,
    CONNECTED,
  } state_t;

public:
  int setTimer(int interval, bool repeat) {
    return timer_.set(interval, repeat);
  }

  void cancelTimer(int id) {
    timer_.cancel(id);
  }

  void cancelAllTimer() {
    timer_.cancelAll();
  }

  void close() {
    if (ev_ != nullptr) {
      event_free(ev_);
      ev_ = nullptr;
    }

    if (fd_ >= 0) {
      evutil_closesocket(fd_);
    }
  }

  void connect() {
    if (state_ != DISCONNECTED) { return; }

    evutil_socket_t fd;
    int rv = z::connect(server_host_, server_port_, fd);
    switch (rv) {
      case OK: {
        state_ = CONNECTED;
        setupReadEvent(fd);
        break;
      }
      case ERR_IO_PENDING: {
        state_ = WAIT_FOR_RSP;
        setupReadEvent(fd);
        break;
      }
      case FAIL:
      default:
        break;
    }
  }

  virtual void onRead(evutil_socket_t fd) override {}

  virtual void onWrite(evutil_socket_t fd) override {}

  virtual void onTimeout(int id) override {}

private:
  void setupReadEvent(evutil_socket_t fd) {
    assert(ev_ == nullptr);
    assert(fd_ < 0);
    fd_ = fd;
    ev_ = event_new(base_, fd, EV_READ|EV_PERSIST, event_callback, this);
  }

private:
  struct event_base *base_;
  const std::string server_host_;
  const unsigned short server_port_;

  evutil_socket_t fd_;
  struct event *ev_;

  state_t state_;

  z::Timer timer_;
};

}


int main(int argc, char *argv[])
{
  log("just a test.");

  struct event_base *base = event_base_new();

  {
    // EventGroup worker;
    z::Server<z::EchoHandler> server(base);

    // setup
    server
        .setHost("0.0.0.0")
        .setPort(1984)
        .setOption(server.BACKLOG, 128)
        .setOption(server.KEEPALIVE, 1);

    // listen
    server.listen();
  }

  // event_base_loop(base, EVLOOP_ONCE);
  log("enter loop");
  event_base_dispatch(base);

  return 0;
}

