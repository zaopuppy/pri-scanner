//
// Created by zhaoyi on 15-7-2.
//

#ifndef PRI_SCANNER_LAYER_STREM_H
#define PRI_SCANNER_LAYER_STREM_H

#include <stddef.h>
#include <unistd.h>
#include <memory>
#include <vector>
#include <assert.h>
#include <string.h>
#include <netinet/in.h>
#include <event2/util.h>
#include "../libframework/zerrno.h"

namespace z {

static const int BUF_SIZE = 2 << 10;

/**
 *
 *
 * +--------+--------+-----------+-+-+
 * | parent | child  | child     | | |
 * |        |        +-----------+ | |
 * |        +----------------------+ |
 * +---------------------------------+
 *
 */
class Layer {
public:
  Layer(Layer *child)
      : child_(child)
  { }

  Layer(): child_(nullptr) {}

  typedef struct {
    char *data;
    int len;
  } buf_t;

public:
  void add(Layer *layer) {
    assert(layer != nullptr);

    if (child_ == nullptr) {
      child_ = layer;
    } else {
      child_->add(layer);
    }
  }

  // TODO: what if the size of `out` is smaller than `in`?
  int read(const buf_t &in, const buf_t &out) {
    if (child_ == nullptr) {
      return decode(in, out);
    }

    // FIXME: not good, need to copy several times and will limit the size of `out`
    buf_t buf = {
        .data = buf_,
        .len = sizeof(buf_),
    };
    int rv = decode(in, buf);
    if (rv <= 0) {
      return rv;
    }
    buf.len = rv;
    return child_->read(buf, out);
  }

  virtual int encode(const buf_t &in, const buf_t &out) = 0;
  virtual int decode(const buf_t &in, const buf_t &out) = 0;

private:
  // TODO: refer `webkit` for usage of smart pointer
  Layer *child_;
  char buf_[BUF_SIZE];
};


class TransparentLayer: public Layer {
public:
  TransparentLayer(Layer *child): Layer(child) {}

public:
  virtual int encode(const buf_t &in, const buf_t &out) {
    return -1;
  }

  virtual int decode(const buf_t &in, const buf_t &out) {
    return -1;
  }
};


/**
 *
 * | fixed size |  2  |
 * +------------+-----+-----------------------
 * | HEAD       | LEN | Data...
 * +------------+-----+-----------------------
 */
class FixedHeadLayer: public Layer {
public:
  FixedHeadLayer(const std::string head, Layer *child)
      : Layer(child)
      , head_(head)
      , state_(WAIT_HEAD)
  { }

  typedef enum {
    WAIT_HEAD,
    WAIT_LEN,
    WAIT_DATA,
  } state_t;

public:
  // FIXME: recusive call, may cause stack overflow
  virtual int decode(const buf_t &in, const buf_t &out) override {
    switch (state_) {
      case WAIT_HEAD: {
        return decodeHead(in, out);
      }
      case WAIT_LEN: {
        return decodeLen(in, out);
      }
      case WAIT_DATA: {
        return decodeData(in, out);
      }
      default: {
        assert(false);
        return -1;
      }
    }
  }

private:
  int decodeHead(const buf_t &in, const buf_t &out) {
    assert(state_ == WAIT_HEAD);

    if (in.len < head_.length()) {
      return 0;
    }

    setState(WAIT_LEN);

    buf_t new_in = {
        .data = in.data + head_.length(),
        .len = in.len - (int)head_.length(),
    };

    return decode(new_in, out);
  }

  int decodeLen(const buf_t &in, const buf_t &out) {
    assert(state_ == WAIT_LEN);

    if (in.len < sizeof(len_)) {
      return 0;
    }

    len_ = ntohs(*(uint16_t*)in.data);

    setState(WAIT_DATA);

    buf_t new_in = {
        .data = in.data + sizeof(len_),
        .len = in.len - (int)sizeof(len_),
    };

    return decode(new_in, out);
  }

  int decodeData(const buf_t &in, const buf_t &out) {
    assert(state_ == WAIT_DATA);

    if (in.len < len_) {
      return 0;
    }

    setState(WAIT_HEAD);

    memcpy(out.data, in.data, len_);

    buf_t new_in = {
        .data = in.data + len_,
        .len = in.len - len_,
    };

    buf_t new_out = {
        .data = out.data + len_,
        .len = out.len - len_,
    };

    return decode(new_in, new_out);
  }

  void setState(state_t new_state) {
    state_ = new_state;
  }

private:
  const std::string head_;
  state_t state_;
  uint16_t len_;
};

class Stream {
public:
  virtual int read(char *buf, int buf_size) = 0;
  virtual int write(char *buf, int buf_size) = 0;
  // virtual void flush() = 0;
};

class LayeredStream: public Stream {
public:
  LayeredStream(evutil_socket_t fd, Layer *layer)
      : fd_(fd)
      , layer_(layer)
  {}

public:
  virtual int read(char *buf, int buf_size) {

    // read raw data
    int rv = (int) ::read(fd_, read_buf_, sizeof(read_buf_));
    if (rv <= 0) {
      return rv;
    }

    Layer::buf_t in = {
        .data = read_buf_,
        .len = rv,
    };

    Layer::buf_t out = {
        .data = buf,
        .len = buf_size,
    };

    // feed pipeline
    return layer_->read(in, out);
  }

  virtual int write(char *buf, int buf_size) {
    //
  }

private:
  evutil_socket_t fd_;
  Layer *layer_;
  char read_buf_[BUF_SIZE];
  // char write_buf_[BUF_SIZE];
};


//void foo()
//{
//  char buf[1024];
//  std::unique_ptr<Stream> stream(new LayeredStream(0, nullptr));
//  int rv = stream->read(buf, sizeof(buf));
//}

}

#endif //PRI_SCANNER_LAYER_STREM_H
