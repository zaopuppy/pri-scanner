//
// Created by Yi Zhao on 5/3/15.
//

#ifndef PRI_SCANNER_ZHTTP_CLIENT_H
#define PRI_SCANNER_ZHTTP_CLIENT_H

#include <string>

#include "zmodule.h"
#include "zerrno.h"

namespace Z {

class HttpClient: public ZModule {

public:
  HttpClient(const char *url, module_type_t type) : ZModule(type) {}

  typedef ZModule super_;

public:
  virtual int init() { return OK; }
  virtual void close() {}
  virtual int onInnerMsg(ZInnerMsg *msg) = 0;
  virtual void routine(long delta) = 0;

  virtual int read(char *buf, int buf_len) {
    //return (int)::recv(fd_, buf, (size_t) buf_len, 0);
    return OK;
  }

  virtual int write(char const *buf, int buf_len) {
    //return (int) ::send(fd_, buf, buf_len, 0);
    return OK;
  }

private:
  std::string url_;
};

}


#endif //PRI_SCANNER_ZHTTP_CLIENT_H
