#include <stdio.h>
#include <string>
#include <future>


#define log(_format_, _args_...) printf(_format_, ##_args_)

class Server {
public:
  Server()
      : host_("0.0.0.0")
      , port_(0)
  {}

  enum {
    SO_BACKLOG,
    SO_KEEPALIVE,
  };

public:
  Server& setHost(const std::string &host) {
    host_.assign(host);
    return *this;
  }

  Server& setPort(short port) {
    port_ = port;
    return *this;
  }

  Server& setOption(uint32_t opt, int value) {}

  Server& listen() {
    return *this;
  }

  std::future<void> closeFuture() {
    return closePromise.future();
  }

private:
  std::string host_;
  short port_;
};


int main(int argc, char *argv[])
{
  log("just a test.");

  {
    Server server;
    server
        .setHost("0.0.0.0")
        .setPort(1984)
        .setOption(Server::SO_BACKLOG, 128)
        .setOption(Server::SO_KEEPALIVE, 1)
        .listen();

    server.closeFuture().wait();
  }

  return 0;
}

