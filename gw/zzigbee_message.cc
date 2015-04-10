#include "zzigbee_message.h"

#include <stdio.h>
#include <assert.h>


#define ZMSG_ENCODE_BEGIN() int rv, encode_len = 0; len_ = getBodyLen()
#define ZMSG_ENCODE_END() return encode_len

#define ZMSG_DECODE_BEGIN() int rv, decode_len = 0
#define ZMSG_DECODE_END() return decode_len

#define ZMSG_ENCODE_SUPER() \
do { \
  rv = super_::encode(buf, buf_len); \
  if (rv < 0) { \
    return rv; \
  } \
  buf += rv; \
  buf_len -= rv; \
  encode_len += rv; \
} while (false)

#define ZMSG_ENCODE(_field) \
do { \
  rv = ::encode(_field, buf, buf_len); \
  if (rv < 0) { \
    return rv; \
  } \
  buf += rv; \
  buf_len -= rv; \
  encode_len += rv; \
} while (false)

#define ZMSG_DECODE_SUPER() \
do { \
  rv = super_::decode(buf, buf_len); \
  if (rv < 0) { \
    return rv; \
  } \
  buf += rv; \
  buf_len -= rv; \
  decode_len += rv; \
} while (false)

#define ZMSG_DECODE(_field) \
do { \
  rv = ::decode(_field, buf, buf_len); \
  if (rv < 0) { \
    return rv; \
  } \
  buf += rv; \
  buf_len -= rv; \
  decode_len += rv; \
} while (false)

///////////////////////////////////////////////////////////////
int ZZigBeeMsg::encode(char* buf, uint32_t buf_len)
{
  // int rv;
  // int encode_len = 0;
  ZMSG_ENCODE_BEGIN();

  // IMP
  // child class must call getEncodeLen() to update length field

  // FIXME: move this out of protocol stack
  {
    const char *sync_bytes = "ZZZZZZZZ";
    const uint32_t sync_bytes_len = sizeof("ZZZZZZZZ") - 1;
    if (buf_len < sync_bytes_len) {
      return -1;
    }

    memcpy(buf, sync_bytes, sync_bytes_len);
    buf += sync_bytes_len;
    buf_len -= sync_bytes_len;
    encode_len += sync_bytes_len;
  }
  // XXX: temporary using

  // ZMSG_ENCODE(syn_);
  ZMSG_ENCODE(ver_);
  ZMSG_ENCODE(len_);
  ZMSG_ENCODE(cmd_);
  ZMSG_ENCODE(addr_);

  ZMSG_ENCODE_END();
}

int ZZigBeeMsg::decode(char* buf, uint32_t buf_len)
{
  // int rv;
  // int decode_len = 0;
  ZMSG_DECODE_BEGIN();

  // ZMSG_DECODE(syn_);
  ZMSG_DECODE(ver_);
  ZMSG_DECODE(len_);
  ZMSG_DECODE(cmd_);
  ZMSG_DECODE(addr_);

  ZMSG_DECODE_END();
  // return decode_len;
}

///////////////////////////////////////////////////////////////
// REG req
ZZBRegReq::ZZBRegReq() : ZZigBeeMsg()
{
  cmd_ = Z_ID_ZB_REG_REQ;
  memset(&mac_.data, 0x00, sizeof(mac_.data));
}

int ZZBRegReq::encode(char* buf, uint32_t buf_len)
{
  ZMSG_ENCODE_BEGIN();

  ZMSG_ENCODE_SUPER();

  ZMSG_ENCODE(mac_);
  ZMSG_ENCODE(dev_type_);

  ZMSG_ENCODE_END();
}

int ZZBRegReq::decode(char* buf, uint32_t buf_len)
{
  ZMSG_DECODE_BEGIN();
  ZMSG_DECODE_SUPER();

  ZMSG_DECODE(mac_);
  ZMSG_DECODE(dev_type_);

  ZMSG_DECODE_END();
}

///////////////////////////////////////////////////////////////
// REG rsp
ZZBRegRsp::ZZBRegRsp() : ZZigBeeMsg()
{
  cmd_ = Z_ID_ZB_REG_RSP;
}

int ZZBRegRsp::encode(char* buf, uint32_t buf_len)
{
  ZMSG_ENCODE_BEGIN();
  ZMSG_ENCODE_SUPER();

  ZMSG_ENCODE(status_);

  ZMSG_ENCODE_END();
}

int ZZBRegRsp::decode(char* buf, uint32_t buf_len)
{
  ZMSG_DECODE_BEGIN();
  ZMSG_DECODE_SUPER();
  ZMSG_DECODE(status_);
  ZMSG_DECODE_END();
}


///////////////////////////////////////////////////////////////
// GET
ZZBGetReq::ZZBGetReq():
  ZZigBeeMsg()
{
  cmd_ = Z_ID_ZB_GET_REQ;
}

int ZZBGetReq::encode(char* buf, uint32_t buf_len) {
  Z_LOG_D("ZZBGetReq::encode()");

  ZMSG_ENCODE_BEGIN();
  ZMSG_ENCODE_SUPER();

  ZMSG_ENCODE(items_);

  ZMSG_ENCODE_END();
}

int ZZBGetReq::decode(char* buf, uint32_t buf_len) {
  Z_LOG_D("ZZBGetReq::decode()");

  ZMSG_DECODE_BEGIN();
  ZMSG_DECODE_SUPER();

  ZMSG_DECODE(items_);

  ZMSG_DECODE_END();
}

///////////////////////////////////////////////////////////////
// GET rsp
ZZBGetRsp::ZZBGetRsp():
  ZZigBeeMsg()
{
  cmd_ = Z_ID_ZB_GET_RSP;
}

int ZZBGetRsp::encode(char* buf, uint32_t buf_len) {
  Z_LOG_D("ZZBGetRsp::encode()");

  ZMSG_ENCODE_BEGIN();
  ZMSG_ENCODE_SUPER();

  ZMSG_ENCODE(items_);

  ZMSG_ENCODE_END();

}

int ZZBGetRsp::decode(char* buf, uint32_t buf_len) {
  Z_LOG_D("ZZBGetRsp::decode()");

  ZMSG_DECODE_BEGIN();
  ZMSG_DECODE_SUPER();

  ZMSG_DECODE(items_);

  ZMSG_DECODE_END();
}

///////////////////////////////////////////////////////////////
// SET
ZZBSetReq::ZZBSetReq():
  ZZigBeeMsg()
{
  cmd_ = Z_ID_ZB_SET_REQ;
}

int ZZBSetReq::encode(char* buf, uint32_t buf_len) {
  Z_LOG_D("ZZBSetReq::encode()");

  ZMSG_ENCODE_BEGIN();
  ZMSG_ENCODE_SUPER();

  ZMSG_ENCODE(items_);

  ZMSG_ENCODE_END();
}

int ZZBSetReq::decode(char* buf, uint32_t buf_len) {
  Z_LOG_D("ZZBSetReq::decode()");

  ZMSG_DECODE_BEGIN();
  ZMSG_DECODE_SUPER();

  ZMSG_DECODE(items_);

  ZMSG_DECODE_END();
}

///////////////////////////////////////////////////////////////
// SET rsp
ZZBSetRsp::ZZBSetRsp():
  ZZigBeeMsg()
{
  cmd_ = Z_ID_ZB_SET_RSP;
}

int ZZBSetRsp::encode(char* buf, uint32_t buf_len) {
  Z_LOG_D("ZZBSetRsp::encode()");

  ZMSG_ENCODE_BEGIN();
  ZMSG_ENCODE_SUPER();

  ZMSG_ENCODE(status_);

  ZMSG_ENCODE_END();
}

int ZZBSetRsp::decode(char* buf, uint32_t buf_len) {
  Z_LOG_D("ZZBSetRsp::decode()");

  ZMSG_DECODE_BEGIN();
  ZMSG_DECODE_SUPER();

  ZMSG_DECODE(status_);

  ZMSG_DECODE_END();
}

//////////////////////////////////////////////////////////////////
// bind
ZZBBindReq::ZZBBindReq()
: ZZigBeeMsg()
{
  cmd_ = Z_ID_ZB_BIND_REQ;
}

int ZZBBindReq::encode(char *buf, uint32_t buf_len)
{
  Z_LOG_D("ZZBBindReq::encode()");

  ZMSG_ENCODE_BEGIN();
  ZMSG_ENCODE_SUPER();

  ZMSG_ENCODE_END();
}

int ZZBBindReq::decode(char *buf, uint32_t buf_len)
{
  Z_LOG_D("ZZBBindReq::decode()");

  ZMSG_DECODE_BEGIN();
  ZMSG_DECODE_SUPER();

  ZMSG_DECODE_END();
}

//////////////////////////////////////////////////////////////////
// bind-rsp
ZZBBindRsp::ZZBBindRsp()
: ZZigBeeMsg()
{
  cmd_ = Z_ID_ZB_BIND_RSP;
}

int ZZBBindRsp::encode(char *buf, uint32_t buf_len)
{
  ZMSG_ENCODE_BEGIN();
  ZMSG_ENCODE_SUPER();

  ZMSG_ENCODE_END();
}

int ZZBBindRsp::decode(char *buf, uint32_t buf_len)
{
  ZMSG_DECODE_BEGIN();
  ZMSG_DECODE_SUPER();

  ZMSG_DECODE_END();
}


//////////////////////////////////////////////////////////////////
// Update
ZZBUpdateIdInfoReq::ZZBUpdateIdInfoReq()
: ZZigBeeMsg()
{
  cmd_ = Z_ID_ZB_UPDATE_ID_REQ;
}

int ZZBUpdateIdInfoReq::encode(char *buf, uint32_t buf_len)
{
  ZMSG_ENCODE_BEGIN();
  ZMSG_ENCODE_SUPER();

  ZMSG_ENCODE(id_list_);

  ZMSG_ENCODE_END();
}

int ZZBUpdateIdInfoReq::decode(char *buf, uint32_t buf_len)
{
  ZMSG_DECODE_BEGIN();
  ZMSG_DECODE_SUPER();

  ZMSG_DECODE(id_list_);

  ZMSG_DECODE_END();
}

ZZBUpdateIdInfoRsp::ZZBUpdateIdInfoRsp()
: ZZigBeeMsg()
{
  cmd_ = Z_ID_ZB_UPDATE_ID_RSP;
}

int ZZBUpdateIdInfoRsp::encode(char *buf, uint32_t buf_len)
{
  ZMSG_ENCODE_BEGIN();
  ZMSG_ENCODE_SUPER();

  ZMSG_ENCODE(status_);

  ZMSG_ENCODE_END();
}

int ZZBUpdateIdInfoRsp::decode(char *buf, uint32_t buf_len)
{
  ZMSG_DECODE_BEGIN();
  ZMSG_DECODE_SUPER();

  ZMSG_DECODE(status_);

  ZMSG_DECODE_END();
}

//////////////////////////////////////////////////////////////////
// broadcast
ZZBBroadcastInd::ZZBBroadcastInd(): ZZigBeeMsg()
{
  cmd_ = Z_ID_ZB_BROADCAST_IND;
}

int ZZBBroadcastInd::encode(char *buf, uint32_t buf_len)
{
  ZMSG_ENCODE_BEGIN();
  ZMSG_ENCODE_SUPER();

  ZMSG_ENCODE(what_);

  ZMSG_ENCODE_END();
}

int ZZBBroadcastInd::decode(char *buf, uint32_t buf_len)
{
  ZMSG_DECODE_BEGIN();
  ZMSG_DECODE_SUPER();

  ZMSG_DECODE(what_);

  ZMSG_DECODE_END();
}


ZZBUploadReq::ZZBUploadReq()
: ZZigBeeMsg()
{
  cmd_ = Z_ID_ZB_UPLOAD_REQ;
}

int ZZBUploadReq::encode(char *buf, uint32_t buf_len) {
  ZMSG_ENCODE_BEGIN();
  ZMSG_ENCODE_SUPER();

  ZMSG_ENCODE(device_id_);
  ZMSG_ENCODE(data_);

  ZMSG_ENCODE_END();
}

int ZZBUploadReq::decode(char *buf, uint32_t buf_len) {
  ZMSG_DECODE_BEGIN();
  ZMSG_DECODE_SUPER();

  ZMSG_DECODE(device_id_);
  ZMSG_DECODE(data_);

  ZMSG_DECODE_END();
}
