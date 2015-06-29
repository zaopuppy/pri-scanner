#include "zclient.h"

#include <errno.h>
#include <assert.h>
// inet_addr
#include <arpa/inet.h>
#include <unistd.h>

#include "zlog_wrapper.h"
#include "zerrno.h"
#include "zdispatcher.h"

// TODO: server wait timeout
// static const struct timeval SERVER_WAIT_TIMEOUT = { 20, 0 };
static const struct timeval RETRY_INTERVAL = { 3, 0 };

// static
void ZClient::socket_callback(evutil_socket_t fd, short events, void *arg) {
  Z_LOG_D("socket_callback(%d, %d)", fd, events);
  assert(arg);
  ZClient *m = (ZClient*)arg;
  m->event(fd, events);
}

// static
//void ZClient::timeout_callback(evutil_socket_t fd, short events, void *arg)
//{
//  Z_LOG_D("timeout_callback(%d, %d)", fd, events);
//  assert(arg);
//  ZClient *m = (ZClient*)arg;
//  m->onTimeoutPrivate();
//}

void ZClient::onTimeout(int id)
{
  Z_LOG_D("ZClient::doTimeout(): %p", this);

  // timeout_event_proxy_.cancel();

  if (id == reconnect_timer_id_) {
    doConnect(-1, EV_TIMEOUT);
  } else if (id == connect_wait_timer_id_) {
    disconnect();
  } else {
    Z_LOG_E("bad timer id: %d", id);
  }

  //switch (state_) {
  //  case STATE_WAITING_FOR_CONNECT:
  //    Z_LOG_I("connecting timeout, abort, waiting for reconnect: %p", this);
  //    disconnect();
  //    break;
  //  case STATE_DISCONNECTED:
  //    doConnect(fd_, EV_TIMEOUT);
  //    break;
  //  default:
  //    break;
  //}

}

int ZClient::init() {
  Z_LOG_I("ZClient::init(): %p", this);

  int rv;

  rv = ZDispatcher::instance()->registerModule(this);
  if (rv != OK) {
    return FAIL;
  }

  // rv = onDisconnected(-1, 0);
  rv = doConnect(-1, 0);
  if (rv != OK && rv != ERR_IO_PENDING) {
    return FAIL;
  }

  if (handler_) {
    handler_->init();
  }

  return OK;
}

int ZClient::onInnerMsg(ZInnerMsg *msg)
{
  Z_LOG_D("ZClient::sendMsg(%p)", msg);
  if (handler_) {
    handler_->onInnerMsg(msg);
  }
  return 0;
}

void ZClient::disconnect() {
  if (fd_ >= 0) {
    ::close(fd_); // TODO: evutil_closesocket(fd_);
    fd_ = -1;
  }

  socket_event_proxy_.cancel();
}

// 0: success, connected
//-1: IO_PENDING
// this method should not change current state;
int ZClient::connect() {
  Z_LOG_D("connect()");

  disconnect();

  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_ < 0) {
    Z_LOG_E("failed to get new socket file descriptor");
    return FAIL;
  }

  evutil_make_socket_nonblocking(fd_);

#ifndef WIN32
  {
    int one = 1;
    if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one))) {
      perror("setsockopt");
      ::close(fd_);
      fd_ = -1;
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
  sin.sin_addr.s_addr = inet_addr(server_ip_.c_str());
  sin.sin_port = htons(server_port_);

  // connect(int socket, const struct sockaddr *address, socklen_t address_len);
  rv = ::connect(fd_, (struct sockaddr*) (&sin), sizeof(sin));
  if (rv < 0) {
    if (errno != EINPROGRESS) {

      perror("connect");
      Z_LOG_D("Can not initial connection");
      ::close(fd_); // XXX: evutil_closesocket
      fd_ = -1;
      return FAIL;
    } else {
      return ERR_IO_PENDING;
    }
  }

  return OK;
}

void ZClient::close() {
  socket_event_proxy_.cancel();

  cancelAllTimer();

  ::close(fd_); // XXX: evutil_closesocket(fd_);
  fd_ = -1;
  state_ = STATE_DISCONNECTED;
}

void ZClient::event(evutil_socket_t fd, short events) {
  // Z_LOG_D("ZClient::event()");
  switch (state_) {
    case STATE_WAITING_FOR_CONNECT:
      onWaitingForConnect(fd, events);
      break;
    case STATE_CONNECTED:
      onConnected(fd, events);
      break;
    case STATE_DISCONNECTED:
      onDisconnected(fd, events);
      break;
    default:
      close();
      break;
  }
}

int ZClient::onWaitingForConnect(evutil_socket_t fd, short events) {
  Z_LOG_D("ZClient::onWaitingForConnect(%d, %d)", fd, events);

  int val = -1;
  socklen_t val_len = sizeof(val);
  int rv = getsockopt(fd, SOL_SOCKET, SO_ERROR, &val, &val_len);
  if (rv == 0 && val == 0) {
    Z_LOG_I("Connected: %p", this);

    cancelTimer(connect_wait_timer_id_);

    socket_event_proxy_.registerSocket(fd, EV_READ|EV_PERSIST, (void*)this, NULL);

    state_ = STATE_CONNECTED;
    if (handler_) {
      Z_LOG_D("handler_->fd_ = %d", fd);
      handler_->setFd(fd);
      handler_->onConnected();
    }
  } else if (rv == 0 && val == EINPROGRESS) { // val is errno, errno won't be set in this situation
    Z_LOG_W("still connecting, rv=%d, val=%d, errno=%d: %p", rv, val, errno, this);
    // one shot event
    socket_event_proxy_.registerSocket(fd, EV_WRITE, this, NULL);
  } else {
    Z_LOG_I("Failed to connect, rv=%d, val=%d, errno=%d: %p", rv, val, errno, this);

    close();
    //::close(fd_);
    //fd_ = -1;
    //state_ = STATE_DISCONNECTED;

    scheduleReconnect();
  }

  return OK;

}

void ZClient::scheduleReconnect() {
  Z_LOG_D("ZClient::scheduleReconnect: %p", this);
  state_ = STATE_DISCONNECTED;
  // cancel old first
  cancelTimer(reconnect_timer_id_);
  //timeout_event_proxy_.cancel();
  //timeout_event_proxy_.registerTimeout(this, &RETRY_INTERVAL);
  reconnect_timer_id_ = setTimer(3*1000, false);
}

void ZClient::onConnected(evutil_socket_t fd, short events) {
  Z_LOG_D("onConnected(%d, %d)", fd, events);
  assert(fd >= 0);

  int rv = read(buf_, sizeof(buf_));
  if (rv == 0) {
    Z_LOG_D("peer closed");
    close();
    scheduleReconnect();
    return;
  } else if (rv < 0) { // XXX EAGAIN
    // perror("recv");
    Z_LOG_D("Failed to receive data from socket: errno=%d", errno);
    close();
    scheduleReconnect();
    return;
  }

  if (handler_) {
    handler_->onRead(buf_, rv);
  }
}

int ZClient::onDisconnected(evutil_socket_t fd, short events) {
  Z_LOG_D("onDisconnected(%d, %d)", fd, events);
  assert(fd >= 0);

  int rv = read(buf_, sizeof(buf_));
  if (rv <= 0) {
    Z_LOG_D("peer closed");
  } else {
    Z_LOG_E("impossible");
    assert(false);
  }

  return OK;
}


int ZClient::doConnect(evutil_socket_t fd, short events) {
  int rv = connect();
  switch (rv) {
    case OK: {
      Z_LOG_D("Connected");
      state_ = STATE_CONNECTED;
      socket_event_proxy_.registerSocket(fd_, EV_READ|EV_PERSIST, (void*)this, NULL);
      if (handler_) {
        Z_LOG_D("handler_->fd_ = %d", fd_);
        handler_->setFd(fd_);
        handler_->onConnected();
      }
      break;
    }
    case ERR_IO_PENDING: {
      Z_LOG_I("Waiting for socket ack: %p", this);
      state_ = STATE_WAITING_FOR_CONNECT;

      // one shot write event
      socket_event_proxy_.registerSocket(fd_, EV_WRITE, (void*)this, NULL);

      // one-shot timer
      // if old timer is still there, remove it first
      cancelTimer(connect_wait_timer_id_);
      // timeout_event_proxy_.registerTimeout(this, &RETRY_INTERVAL);
      connect_wait_timer_id_ = setTimer(3*1000, false);
      assert(connect_wait_timer_id_ >=0);
      break;
    }
    case FAIL: {
      Z_LOG_D("Failed to connect");
      state_ = STATE_DISCONNECTED;

      scheduleReconnect();

      break;
    }
    default: {
      rv = FAIL;
      break;
    }
  }

  return rv;
}
