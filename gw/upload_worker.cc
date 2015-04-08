//
// Created by Yi Zhao on 4/8/15.
//

#include "upload_worker.h"
#include "zinner_message_ex.h"

#include <stdio.h>
#include <unistd.h>
#include <libframework/zerrno.h>

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
  return FAIL;
}

int UploadWorker::handleConfigReq(ZInnerConfigReq *req) {
  return FAIL;
}
