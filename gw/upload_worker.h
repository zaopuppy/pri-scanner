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
  typedef Z::Thread super_;

  virtual bool init() override;

  UploadWorker(): stop_(false), max_sending_queue_length_(4096) {}

  int send(ZInnerMsg *msg);

  virtual void run();

private:
  int handleUploadReq(ZInnerUploadReq * req);

  int handleConfigReq(ZInnerConfigReq *req);

  bool sendingQueueIsFull();

  int save(ZInnerUploadReq *req);

  int addToSendingQueue(ZInnerUploadReq *req);

  bool loadFromDisk();

private:
  bool stop_;

  std::vector<ZInnerMsg*> msg_queue_;



  const int max_sending_queue_length_;
  std::vector<ZInnerUploadReq*> sendingQueue_;

};


#endif //MONSYS_UPLOAD_WORKER_H
