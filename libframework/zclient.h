#ifndef _ZCLIENT_H__
#define _ZCLIENT_H__

#include <event2/event.h>
#include <iostream>

#include "zmodule.h"
#include "zinner_message.h"
#include "zclient_handler.h"
#include "zevent_proxy.h"

class ZClient : public ZModule {
public:
  ZClient(event_base *base, int type)
      : ZModule(type)
      , /*base_(base), */fd_(-1)
      , socket_event_proxy_(base, ZClient::socket_callback)
      , timeout_event_proxy_(base, ZClient::timeout_callback)
      , server_ip_("0.0.0.0"), server_port_(0), handler_(NULL)
  {
  }

  typedef ZModule super_;

public:
  virtual int init();
  virtual void close();
  // virtual int sendMsg(ZInnerMsg *msg);
  virtual int onInnerMsg(ZInnerMsg *msg);

  // TODO
  // virtual void onTimeout(int id) {}
  virtual bool isActive() { return state_ == STATE_CONNECTED; }

  void event(evutil_socket_t fd, short events);
  void setServerAddress(const char *server_ip, unsigned short server_port) {
    server_ip_ = server_ip;
    server_port_ = server_port;
  }

  virtual int read(char *buf, int buf_len) {
    return (int)::recv(fd_, buf, (size_t) buf_len, 0);
  }

  virtual int write(char const *buf, int buf_len) {
    return (int) ::send(fd_, buf, buf_len, 0);
  }


protected:
  // for state-transient
  int onWaitingForConnect(evutil_socket_t fd, short events);
  void onConnected(evutil_socket_t fd, short events);
  int onDisconnected(evutil_socket_t fd, short events);

  // triggle `connect` but don't handle asynchronized event
  int connect();
  // call `connect` and setup event if neccessary
  int doConnect(evutil_socket_t fd, short events);
  void disconnect();
  void scheduleReconnect();

  void setHandler(ZClientHandler *handler) { handler_ = handler; }

  void onTimeoutPrivate();

  static void socket_callback(evutil_socket_t fd, short events, void *arg);
  static void timeout_callback(evutil_socket_t fd, short events, void *arg);

private:
  enum STATE {
    STATE_WAITING_FOR_CONNECT,
    STATE_CONNECTED,
    STATE_DISCONNECTED,
  };

private:
  evutil_socket_t fd_;
  ZEventProxy socket_event_proxy_;
  ZEventProxy timeout_event_proxy_;
  STATE state_;
  char buf_[1 << 10];
  // int type_;
  std::string server_ip_;
  unsigned short server_port_;

  ZClientHandler *handler_;

  // time_t timeout_; // in seconds
};

#endif // _ZCLIENT_H__

