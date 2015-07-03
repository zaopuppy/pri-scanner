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
#include "../libframework/zerrno.h"

namespace z {

static const int BUF_SIZE = 10 << 10;

/**
 *
 *
 * +--------+--------+-----------+-+-+
 * | parent | child1 | child     | | |
 * |        |        +-----------+ | |
 * |        +----------------------+ |
 * +---------------------------------+
 *
 */
class Layer {
public:
  Layer(std::shared_ptr<Layer> child)
      : child_(child)
  { }

  Layer(): child_(nullptr) {}

  typedef struct {
    char *data;
    int len;
  } data_range_t;

public:
  void add(std::shared_ptr<Layer> layer) {
    assert(layer != nullptr);

    if (child_.get() == nullptr) {
      child_ = layer;
    } else {
      child_->add(layer);
    }
  }

  int read(const data_range_t &in, data_range_t &out) {
    if (child_.get() == nullptr) {
      return decode(in, out);
    }

    // FIXME: not good, need to copy several times and will limit the size of `out`
    data_range_t buf = {
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

  virtual int decode(const data_range_t &in, data_range_t &out) = 0;

private:
  // TODO: refer `webkit` for usage of smart pointer
  std::shared_ptr<Layer> child_;
  char buf_[BUF_SIZE];
};

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
  virtual int decode(const data_range_t &in, data_range_t &out) override {
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
  const std::string head_;
  state_t state_;
};

class Stream {
public:
  virtual int read(char *buf, int buf_size) = 0;
  virtual int write(char *buf, int buf_size) = 0;
  // virtual void flush() = 0;
};

class LayeredStream: public Stream {
public:
  virtual int read(char *buf, int buf_size) {

    // read raw data
    int rv = (int) ::read(fd_, read_buf_, sizeof(read_buf_));
    if (rv <= 0) {
      return rv;
    }

    Layer::data_range_t in = {
        .data = read_buf_,
        .len = rv,
    };

    Layer::data_range_t out = {
        .data = buf,
        .len = buf_size,
    };

    // feed pipeline
    return layer_.read(in, out);
  }

  virtual int write(char *buf, int buf_size) {
    //
  }

private:
  int fd_;
  Layer layer_;
  char read_buf_[BUF_SIZE];
  // char write_buf_[BUF_SIZE];
};


void foo()
{
  char buf[1024];
  std::unique_ptr<Stream> stream(new LayeredStream);
  int rv = stream->read(buf, sizeof(buf));
}

}

#endif //PRI_SCANNER_LAYER_STREM_H
