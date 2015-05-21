//
// Created by zhaoyi on 15-5-14.
//

#ifndef PRI_SCANNER_CRAWLER_H
#define PRI_SCANNER_CRAWLER_H

#include <string>
#include <assert.h>
#include <memory>

#include <event2/event.h>
#include <event2/dns.h>

namespace Z {


class Uri {
public:
  std::string scheme_;
  std::string host_;
  unsigned short port_;
  std::string path_;
  std::string query_str_;
};


class Crawler {
public:
  Crawler(struct event_base *base,
          struct evdns_base *dns_base,
          const char *url,
          int thread_num,
          int max_task_num);

  bool init();

  void start();

private:
  struct event_base *base_;
  struct evdns_base *dns_base_;
  const std::string url_;
  std::shared_ptr<Uri> uri_;
  const int thread_num_;
  const int max_task_num_;

};

}

#endif //PRI_SCANNER_CRAWLER_H
