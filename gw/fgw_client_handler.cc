#include "fgw_client_handler.h"

#include <unistd.h>

using namespace com::letsmidi::monsys::protocol::push;

void FGWClientHandler::setState(int new_state)
{
  Z_LOG_D("changed from [%d] to [%d]", state_, new_state);
  state_ = new_state;
}

int FGWClientHandler::init()
{
  addr_.module_type_ = getModuleType();
  addr_.handler_id_ = getId();

  state_ = STATE_UNREGISTERED;

  return OK;
}

int FGWClientHandler::onRead(char *buf, uint32_t buf_len)
{
  Z_LOG_D("FGWClientHandler::onRead()");
  trace_bin(buf, buf_len);

  switch (state_) {
    case STATE_UNREGISTERED:
      return onRead_Unregistered(buf, buf_len);
    case STATE_WAIT_FOR_SERVER:
      return onRead_WaitForServer(buf, buf_len);
    case STATE_REGISTERED:
      return onRead_Registered(buf, buf_len);
    default:
      return FAIL;
  }
}

int FGWClientHandler::onInnerMsg(ZInnerMsg *msg)
{
  Z_LOG_D("FGWClientHandler::onInnerMsg");
  Z_LOG_D("inner message type: %d", msg->msg_type_);

  uint32_t inner_seq = msg->seq_;

  FGWClientSession *session = session_ctrl_.findByKey1(inner_seq);
  if (session == NULL) {
    Z_LOG_E("Failed to find session by inner key: %u", inner_seq);
    return FAIL;
  }

  session->event(msg);
  if (session->isComplete()) {
    session_ctrl_.removeByKey1(inner_seq);
    delete session;
    session = NULL;
  }

  return OK;
}

void FGWClientHandler::routine(long delta)
{
  FGWClientSession *session;

  SESSION_CTRL_TYPE::iterator iter = session_ctrl_.begin();
  SESSION_CTRL_TYPE::iterator tmp_iter;

  while (iter != session_ctrl_.end()) {
    session = iter->second;
    session->doTimeout(delta);
    if (session->isComplete()) {
      tmp_iter = iter;
      ++iter;

      delete tmp_iter->second;
      session_ctrl_.erase(tmp_iter);

    } else {
      ++iter;
    }
  }

}

void FGWClientHandler::login()
{
  Z_LOG_D("FGWClientHandler::registerFGW()");

  PushMsg push_msg;
  push_msg.set_version(PUSH_MSG_VERSION);
  push_msg.set_type(PUSH_CLIENT_LOGIN);
  push_msg.set_sequence(0);

  PushClientLogin *login = new PushClientLogin();
  login->set_device_id("DEVID-Z");

  push_msg.set_allocated_push_client_login(login);

  int rv = protobuf_encode(&push_msg, out_buf_, sizeof(out_buf_));
  if (rv < 0) {
    Z_LOG_E("Failed to encode push msg");
    return;
  }

  getModule()->write(out_buf_, rv);

  state_ = STATE_WAIT_FOR_SERVER;

  // set timer
  if (login_timer_id >= 0) {
    timer_.cancel(login_timer_id);
  }
  login_timer_id = timer_.set(10 * 1000);
  if (login_timer_id < 0) {
    Z_LOG_E("Failed to set timer, there's nothing we can do now");
  }
  Z_LOG_D("login_timer_id=%d", login_timer_id);
}

void FGWClientHandler::close()
{
  setState(STATE_UNREGISTERED);
  // TODO: cancel timer
  // timer_.cancelAll();

  event_free(read_event_);
  read_event_ = NULL;
  ::close(fd_);
  fd_ = -1;
}

int FGWClientHandler::onRead_Unregistered(char *buf, uint32_t buf_len)
{
  Z_LOG_D("FGWClientHandler::onRead_Unregistered()");

  // return not-register
  sendRsp("Not-register(i)", 404);

  return OK;
}

int FGWClientHandler::processLoginRsp(PushMsg *push_msg)
{
  Z_LOG_D("FGWClientHandler::processLoginRsp()");
  if (!push_msg->has_push_client_login_rsp()) {
    return -1;
  }

  const PushClientLoginRsp &rsp = push_msg->push_client_login_rsp();
  if (rsp.code() != 0) {
    Z_LOG_E("Failed to login to server, result is %d", rsp.code());
    return FAIL;
  }

  Z_LOG_D("Good, logged in success");

  setState(STATE_REGISTERED);

  // cancel timer
  timer_.cancel(login_timer_id);
  login_timer_id = -1;

  return OK;
}

// XXX: continus package
int FGWClientHandler::onRead_WaitForServer(char *buf, uint32_t buf_len)
{
  Z_LOG_D("FGWClientHandler::onRead_WaitForServer()");

  PushMsg push_msg;

  int rv = protobuf_decode(&push_msg, buf, buf_len);
  Z_LOG_I("decoded(%d)", rv);
  if (rv < 0) {
    Z_LOG_E("Failed to decode push msg");
    return FAIL;
  }

  if (push_msg.type() != PUSH_CLIENT_LOGIN_RSP) {
    Z_LOG_E("Unknown push msg type: %d", push_msg.type());
    return FAIL;
  }

  return processLoginRsp(&push_msg);
}

int FGWClientHandler::onRead_Registered(char *buf, uint32_t buf_len)
{
  Z_LOG_D("FGWClientHandler::onRead_Registered()");

  if (buf_len <= 0) { // MIN_MSG_LEN(header length)
    Z_LOG_D("empty message");

    sendRsp("empty message", 404);

    return -1;
  }

  // --------------------------------
  PushMsg push_msg;

  int rv = protobuf_decode(&push_msg, buf, buf_len);
  if (rv < 0) {
    Z_LOG_E("Failed to decode push msg");
    return FAIL;
  }

  Z_LOG_D("push msg type=%s received", push_msg.GetTypeName().c_str());

  {
    FGWClientSession *s = session_ctrl_.findByKey2(push_msg.sequence());
    if (s != NULL) {
      Z_LOG_E("Duplicated session, external key: %u", push_msg.sequence());
      return FAIL;
    }
  }

  uint32_t inner_seq = id_generator_.next();
  FGWClientForwardSession *session = new FGWClientForwardSession(this, inner_seq);
  if (!session->init()) {
    return FAIL;
  }

  session->event(&push_msg);
  if (!session->isComplete()) {
    session_ctrl_.add(inner_seq, push_msg.sequence(), session);
  }

  return OK;
}

void FGWClientHandler::onConnected()
{
  Z_LOG_D("FGWClientHandler::onConnected()");

  // reset state
  setState(STATE_UNREGISTERED);

  login();
}

void FGWClientHandler::sendRsp(const char *text_msg, int status)
{
  Z_LOG_D("FGWClientHandler::sendRsp");
  int rv = getModule()->write(text_msg, (int) strlen(text_msg));
  Z_LOG_D("sent %d bytes", rv);
}

void FGWClientHandler::onTimeout(int handler_id)
{
  Z_LOG_D("FGWClientHandler::onTimeout(%d)", handler_id);
  if (handler_id == login_timer_id) {
    if (state_ == STATE_WAIT_FOR_SERVER) {
      setState(STATE_UNREGISTERED);
      login();
    }
  } else {
    Z_LOG_E("unknown handler id: %d", handler_id);
  }
}

