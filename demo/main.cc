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
  void init(Channel ch) {
  }
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

class Server: public EventCallback {
public:
  Server(struct event_base *base)
      : host_("0.0.0.0"), port_(0)
      , base_(base)
      , handler_(nullptr)
      , child_handler_(nullptr)
  { }

  enum {
    BACKLOG,
    KEEPALIVE,
  };

public:
  Server &setHost(const std::string &host) {
    host_.assign(host);
    return *this;
  }

  Server &setPort(unsigned short port) {
    port_ = port;
    return *this;
  }

  Server &setOption(uint32_t opt, int value) { }

  Server &setHandler(std::unique_ptr<Handler> h) {
    handler_ = std::move(h);
    return *this;
  }

  Server &setChildHandler(std::unique_ptr<Handler> h) {
    child_handler_ = std::move(h);
    return *this;
  }

  // ---

  Server &listen() {

    assert(handler_.get() != nullptr);
    assert(child_handler_.get() != nullptr);

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
    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    int cli_fd = ::accept(fd, (struct sockaddr*) (&ss), &slen);
    if (cli_fd < 0) {           // XXX EAGAIN?
      perror("accept");
      ::evutil_closesocket(cli_fd);
      return;
    }

    //
    struct sockaddr_in* addr = (struct sockaddr_in*)(&ss);
    // FIXME: release the allocated memory in a multi-thread program
    std::string addr_str = inet_ntoa(addr->sin_addr);
    unsigned short port = ntohs(addr->sin_port);
    log("accepted connection from: %s:%u", addr_str.c_str(), port);

    evutil_make_socket_nonblocking(cli_fd);

    //
    struct event *ev = event_new(
        base_, cli_fd, EV_READ|EV_PERSIST, event_callback, child_handler_.get());

  }

  virtual void onWrite(evutil_socket_t fd) {
    // ignore
  }

  virtual void onTimeout() {
    //
  }

private:
  std::string host_;
  unsigned short port_;
  struct event_base *base_;
  std::unique_ptr<Handler> handler_;
  std::unique_ptr<Handler> child_handler_;
};

}

/**
 * std::function<int(int)> f = [](int a) { return 1; }
 */
int main(int argc, char *argv[])
{
  log("just a test.");

  struct event_base *base = event_base_new();

  {
    // EventGroup worker;
    z::Server server(base);

    // setup
    server
        .setHost("0.0.0.0")
        .setPort(1984)
        .setOption(z::Server::BACKLOG, 128)
        .setOption(z::Server::KEEPALIVE, 1)
        .setHandler(std::unique_ptr<z::Handler>(nullptr))
        .setChildHandler(std::unique_ptr<z::Handler>(nullptr));

    // listen
    server.listen();
  }

  // event_base_loop(base, EVLOOP_ONCE);
  log("enter loop");
  event_base_dispatch(base);

  return 0;
}

