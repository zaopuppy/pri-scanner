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


#define log(_format_, _args_...) printf(_format_ "\n", ##_args_)

namespace z {


class EventCallback {
public:
  virtual void onRead(evutil_socket_t fd) = 0;
  virtual void onWrite(evutil_socket_t fd) = 0;
  virtual void onTimeout() = 0;
};


class Handler: public EventCallback {
public:
  Handler(evutil_socket_t fd,
          const std::string server_host, unsigned short server_port,
          const std::string client_host, unsigned short client_port)
      : fd_(fd)
      , server_host_(server_host), server_port_(server_port)
      , client_host_(client_host), client_port_(client_port)
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
  virtual void onRead(evutil_socket_t fd) {
    log("onRead(%d)", fd);
  }

  virtual void onWrite(evutil_socket_t fd) {
    log("onWrite(%d)", fd);
  }

  virtual void onTimeout() {
    log("onTimeout");
  }

protected:

  virtual void close() {
    assert(ev_ != nullptr);
    if (ev_ != nullptr) {
      event_free(ev_);
    }
    ::close(fd_);
  }


private:
  const evutil_socket_t fd_;
  struct event *ev_;
  const std::string server_host_;
  const unsigned short server_port_;
  const std::string client_host_;
  const unsigned short client_port_;
};

class EchoHandler: public Handler {
public:
  EchoHandler(evutil_socket_t fd,
              const std::string server_host, unsigned short server_port,
              const std::string client_host, unsigned short client_port)
      : Handler(fd, server_host, server_port, client_host, client_port)
  {}

  typedef Handler super_;

public:
  void onRead(evutil_socket_t fd) {
    int rv = (int) ::read(fd, buf_, sizeof(buf_));
    if (0 == rv) {
      log("peer closed");
      close();
      return;
    } else if (rv < 0) {
      perror("read");
      close();
      return;
    }
    std::string s(buf_, (size_t) rv);
    log("read: (%d, [%s]", rv, s.c_str());

    ::write(fd, buf_, rv);
  }

private:
  char buf_[10 << 10];
};

static void event_callback(evutil_socket_t fd, short events, void* arg) {
  assert(arg != nullptr);

  EventCallback *callback = (EventCallback *) arg;

  if (events & EV_READ) {
    callback->onRead(fd);
  }

  if (events & EV_WRITE) {
    callback->onWrite(fd);
  }

  // FIXME: use ZTimer istead of this.
  if (events & EV_TIMEOUT) {
    callback->onTimeout();
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
static evutil_socket_t accept(evutil_socket_t fd, std::string &host, unsigned short &port) {
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
  host.assign(inet_ntoa(addr->sin_addr));
  port = ntohs(addr->sin_port);

  evutil_make_socket_nonblocking(cli_fd);

  return cli_fd;
}


template <typename CHILD_HANDLER_T>
class Server: public EventCallback {
public:
  Server(struct event_base *base)
      : host_("0.0.0.0"), port_(0)
      , base_(base)
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

protected:
  virtual void onRead(evutil_socket_t fd) {

    std::string client_host;
    unsigned short client_port;
    evutil_socket_t client_fd = z::accept(fd, client_host, client_port);
    if (client_fd < 0) {
      // failed
      return;
    }

    log("accepted connection from: %s:%u", client_host.c_str(), client_port);

    CHILD_HANDLER_T *child_handler = new CHILD_HANDLER_T(
        client_fd, host_, port_, client_host, client_port);
    struct event *ev = event_new(
        base_, client_fd, EV_READ|EV_PERSIST, event_callback, child_handler);
    child_handler->setEvent(ev);

    event_add(ev, nullptr);

  }

private:
  std::string host_;
  unsigned short port_;
  struct event_base *base_;
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

