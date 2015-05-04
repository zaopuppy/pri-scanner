//
// Created by Yi Zhao on 4/8/15.
//

#include "upload_worker.h"
#include "zinner_message_ex.h"

#include <stdio.h>
#include <unistd.h>
#include <libframework/zframework.h>


class ScanRecord {
public:
  int gw_id_;
  int scanner_id_;
  long time_; // can we get this
  int type_;
  std::string data_;
};

class ScanDataManager {
public:
  int size() { return (int) queue_.size(); }
  void push(ScanRecord *rec) { return queue_.push(rec); }
  ScanRecord* pop() { return queue_.front(); }

private:
  std::queue<ScanRecord*> queue_;
};

void UploadWorker::run() {
  for (; !stop_;) {
    sleep(1);
    printf("hi~~~~~\n");
  }
}

int UploadWorker::send(ZInnerMsg *msg) {
  if (msg == NULL) {
    return FAIL;
  }

  // TODO: thread-safe operation
  if (msg_queue_.size() >= max_msg_queue_length_) {
    return -1;
  }

  msg_queue_.push_back(msg);

  switch (msg->msg_type_) {
    case Z_ZB_UPLOAD_REQ:
      return handleUploadReq((ZInnerUploadReq*)msg);
    case Z_ZB_CONFIG_REQ:
      return handleConfigReq((ZInnerConfigReq*)msg);
    default:
      return FAIL;
  }
}

int UploadWorker::handleUploadReq(ZInnerUploadReq *req) {
  int rv;
  if (sendingQueueIsFull()) {
    Z_LOG_W("sending queue is full, try save to local storage");
    rv = save(req);
    if (rv != OK) {
      Z_LOG_E("failed to save upload request to local storage: %d", rv);
      return FAIL;
    }
  }

  return addToSendingQueue(req);
}

int UploadWorker::handleConfigReq(ZInnerConfigReq *req) {
  return FAIL;
}

bool UploadWorker::sendingQueueIsFull() {
  return sendingQueue_.size() >= max_msg_queue_length_;
}

int UploadWorker::save(ZInnerUploadReq *req) {
  return 0;
}

int UploadWorker::addToSendingQueue(ZInnerUploadReq *req) {
  sendingQueue_.push_back(req);
  return 0;
}

bool UploadWorker::init() {
  if (!super_::init()) {
    return false;
  }

  if (!loadFromDisk()) {
    return false;
  }

  return true;
}

bool UploadWorker::loadFromDisk() {
  return false;
}
