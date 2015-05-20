//
// Created by zhaoyi on 15-5-14.
//

#ifndef PRI_SCANNER_CRAWLER_H
#define PRI_SCANNER_CRAWLER_H

#include <string>
#include <assert.h>

#include <event2/event.h>

namespace Z {

class Crawler {
public:
  Crawler(struct event_base *base, const char *url, int thread_num, int max_task_num);

  bool init();

  void start();

private:
  struct event_base *base_;
  const std::string url_;
  const int thread_num_;
  const int max_task_num_;

};

}

#endif //PRI_SCANNER_CRAWLER_H
