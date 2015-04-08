#ifndef Z__INNER_MESSAGE_EX_H__
#define Z__INNER_MESSAGE_EX_H__

#include "libframework/zframework.h"

#include "zzigbee_message.h"
#include "zbdefines.h"

enum {
  Z_INVALID = -1,

  Z_ZB_GET_DEV_LIST_REQ,
  Z_ZB_GET_DEV_LIST_RSP,

  Z_ZB_GET_DEV_REQ,
  Z_ZB_GET_DEV_RSP,

  Z_ZB_SET_DEV_REQ,
  Z_ZB_SET_DEV_RSP,

  Z_ZB_PRE_BIND_REQ,
  Z_ZB_PRE_BIND_RSP,

  Z_ZB_BIND_REQ,
  Z_ZB_BIND_RSP,

  Z_ZB_UPLOAD_REQ,
  Z_ZB_UPLOAD_RSP,

  Z_ZB_CONFIG_REQ,
  Z_ZB_CONFIG_RSP,
};

////////////////////////////////////////////////////
// GetDevList
class ZInnerGetDevListReq : public ZInnerMsg {
public:
  ZInnerGetDevListReq()
      : ZInnerMsg(Z_ZB_GET_DEV_LIST_REQ) { }
public:
  std::string uid_;
  std::string fgw_;
};

class ZInnerGetDevListRsp : public ZInnerMsg {
public:
  ZInnerGetDevListRsp()
      : ZInnerMsg(Z_ZB_GET_DEV_LIST_RSP) { }
  ~ZInnerGetDevListRsp()
  {
    // delete infos
    for (uint32_t i = 0; i < info_list_.size(); ++i) {
      delete info_list_[i];
    }
    info_list_.clear();
  }

public:
  int32_t status_;
  std::vector<ZZBDevInfo*> info_list_;
};

////////////////////////////////////////////////////
// GetDevInfo
class ZInnerGetDevInfoReq : public ZInnerMsg {
public:
  ZInnerGetDevInfoReq()
      : ZInnerMsg(Z_ZB_GET_DEV_REQ) { }

public:
  std::string fgw_;
  std::string uid_;
  uint16_t addr_;
  std::vector<uint8_t> item_ids_;
};

class ZInnerGetDevInfoRsp : public ZInnerMsg {
public:
  ZInnerGetDevInfoRsp()
      : ZInnerMsg(Z_ZB_GET_DEV_RSP) { }

public:
  int32_t status_;
  std::vector<ZItemPair> dev_infos_;
};

////////////////////////////////////////////////////
// SetDevInfo
class ZInnerSetDevInfoReq : public ZInnerMsg {
public:
  ZInnerSetDevInfoReq()
      : ZInnerMsg(Z_ZB_SET_DEV_REQ) { }

public:
  std::string fgw_;
  std::string uid_;
  uint16_t addr_;
  std::vector<ZItemPair> dev_vals_;
};

class ZInnerSetDevInfoRsp : public ZInnerMsg {
public:
  ZInnerSetDevInfoRsp()
      : ZInnerMsg(Z_ZB_SET_DEV_RSP) { }

public:
  uint8_t status_;
};

////////////////////////////////////////////////////
// PreBind
class ZInnerPreBindReq : public ZInnerMsg {
public:
  ZInnerPreBindReq()
      : ZInnerMsg(Z_ZB_PRE_BIND_REQ) { }

public:
  std::string fgw_;
};

class ZInnerPreBindRsp : public ZInnerMsg {
public:
  ZInnerPreBindRsp()
      : ZInnerMsg(Z_ZB_PRE_BIND_RSP) { }

public:
  uint8_t result_;
};

////////////////////////////////////////////////////
// Bind
class ZInnerBindReq : public ZInnerMsg {
public:
  ZInnerBindReq()
      : ZInnerMsg(Z_ZB_BIND_REQ) { }

public:
  std::string fgw_;
};

class ZInnerBindRsp : public ZInnerMsg {
public:
  ZInnerBindRsp()
      : ZInnerMsg(Z_ZB_BIND_RSP) { }

public:
  uint8_t result_;
};

class ZInnerUploadReq: public ZInnerMsg {
public:
  ZInnerUploadReq()
      : ZInnerMsg(Z_ZB_UPLOAD_REQ) {}

public:
  std::vector<std::string> data_list_;
};

class ZInnerUploadRsp: public ZInnerMsg {
public:
  ZInnerUploadRsp()
      : ZInnerMsg(Z_ZB_UPLOAD_RSP) {}

public:
  uint8_t result_;
};

class ZInnerConfigReq: public ZInnerMsg {
public:
  ZInnerConfigReq()
      : ZInnerMsg(Z_ZB_CONFIG_REQ) {}
};


class ZInnerConfigRsp: public ZInnerMsg {
public:
  ZInnerConfigRsp()
      : ZInnerMsg(Z_ZB_CONFIG_RSP) {}

public:
  uint8_t result_;
};


#endif // _Z_INNER_MESSAGE_EX_H__

