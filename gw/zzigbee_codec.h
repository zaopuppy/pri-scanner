#ifndef _Z_ZIGBEE_CODEC_H__
#define _Z_ZIGBEE_CODEC_H__


#include "zbdefines.h"


///////////////////////////////////////////////////////////////
// zb_item_id_info_t
template<>
inline int encode(const zb_item_id_info_t &v, char *buf, uint32_t buf_len)
{
  int rv;
  int len = 0;

  // id
  rv = encode(v.id_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  // name
  rv = encode(v.name_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  // desc
  rv = encode(v.desc_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  // type
  rv = encode(v.type_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  // formatter
  rv = encode(v.formatter_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  return len;
}

template<>
inline int decode(zb_item_id_info_t &v, char *buf, uint32_t buf_len)
{
  int rv;
  int len = 0;

  // id
  rv = decode(v.id_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  // name
  rv = decode(v.name_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  // desc
  rv = decode(v.desc_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  // type
  rv = decode(v.type_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  // formatter
  rv = decode(v.formatter_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  return len;
}

template<>
inline int getlen(const zb_item_id_info_t &v)
{
  return getlen(v.id_)
    + getlen(v.name_)
    + getlen(v.desc_)
    + getlen(v.type_)
    + getlen(v.formatter_);
}

///////////////////////////////////////////////////////////////
// ZZBHeader
template<>
inline int encode(const ZZBHeader &v, char *buf, uint32_t buf_len)
{
  int rv, len = 0;

  rv = encode(v.syn_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  rv = encode(v.ver_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  rv = encode(v.len_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  rv = encode(v.cmd_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  rv = encode(v.addr_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  return len;
}

template<>
inline int decode(ZZBHeader &v, char *buf, uint32_t buf_len)
{
  int rv, len = 0;

  rv = decode(v.syn_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  rv = decode(v.ver_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  rv = decode(v.len_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  rv = decode(v.cmd_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  rv = decode(v.addr_, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  return len;
}

template<>
inline int getlen(const ZZBHeader &v)
{
  return getlen(v.syn_)
    + getlen(v.ver_)
    + getlen(v.len_)
    + getlen(v.cmd_)
    + getlen(v.addr_);
}

template<>
inline int encode(const zb_mac_type_t &v, char *buf, uint32_t buf_len)
{
  uint32_t len = sizeof(v.data);
  if (buf_len < len) {
    return -1;
  }

  memcpy(buf, (void*)v.data, len);

  return len;
}

template<>
inline int decode(zb_mac_type_t &v, char *buf, uint32_t buf_len)
{
  uint32_t len = sizeof(v.data);
  if (buf_len < len) {
    return -1;
  }

  memcpy((void*)v.data, buf, len);

  return len;
}

template<>
inline int getlen(const zb_mac_type_t &v)
{
  return (int)sizeof(v.data);
}

template<>
inline int encode(const ZItemPair &v, char *buf, uint32_t buf_len)
{
  int rv, len = 0;

  rv = encode(v.id, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  rv = encode(v.val, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  return len;
}

template<>
inline int decode(ZItemPair &v, char *buf, uint32_t buf_len)
{
  int rv, len = 0;

  rv = decode(v.id, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  rv = decode(v.val, buf, buf_len);
  if (rv < 0) return rv;
  buf += rv;
  buf_len -= rv;
  len += rv;

  return len;
}

template<>
inline int getlen(const ZItemPair &v)
{
  int len = 0;

  len += getlen(v.id);
  len += getlen(v.val);

  return len;
}


#endif // _Z_ZIGBEE_CODEC_H__


