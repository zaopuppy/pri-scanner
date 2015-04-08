//
// Created by Yi Zhao on 4/8/15.
//

#ifndef MONSYS_UPLOAD_WORKER_H
#define MONSYS_UPLOAD_WORKER_H


#include <libframework/thread.h>
#include <libframework/zinner_message.h>
#include "zinner_message_ex.h"

class UploadWorker: public Z::Thread {

public:
  UploadWorker(): stop_(false) {}

  int send(ZInnerMsg *msg);

protected:
  void run();

private:
  bool stop_;

  int handleUploadReq(ZInnerUploadReq * req);

  int handleConfigReq(ZInnerConfigReq *req);
};


#endif //MONSYS_UPLOAD_WORKER_H
