#ifndef _Z_CLIENT_HANDLER_H__
#define _Z_CLIENT_HANDLER_H__

#include <unistd.h>

#include "zhandler.h"

class ZClientHandler : public ZHandler, public ZTimer::TimerCallback {
public:
  ZClientHandler(int id, ZModule *module, struct event_base *base)
      : ZHandler(id, module)
      , fd_(-1)
      // , read_event_(NULL)
      , timer_(base, this)
  {}

public:
  virtual void close() {
    //event_free(read_event_);
    //read_event_ = NULL;
    ::close(fd_);
    fd_ = -1;
  }

  virtual void onConnected() = 0;

  virtual void onTimeout(int handler_id) { }

public:
  int setTimer(int interval, bool repeat = false) {
    return timer_.set(interval, repeat);
  }

  void cancelTimer(int id) {
    timer_.cancel(id);
  }

  void cancelAllTimer() {
    timer_.cancelAll();
  }

  // FIXME: don't use like this, should always initialize it in constructor
  void setFd(evutil_socket_t fd) {
    fd_ = fd;
  }

  evutil_socket_t getFd() {
    return fd_;
  }

private:
  evutil_socket_t fd_;
  // struct event *read_event_;
  ZTimer timer_;
};

#endif // _Z_CLIENT_HANDLER_H__

