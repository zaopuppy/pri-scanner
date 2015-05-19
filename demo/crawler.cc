//
// Created by zhaoyi on 15-5-14.
//

#include "crawler.h"

#include <event2/http.h>


int main(int argc, char *argv[])
{
  struct event_base *base = event_base_new();
  assert(base != NULL);

  // crawler with 4 threads, 1000 parallel tasks
  Crawler crawler(base, "http://www.baidu.com", 4, 1000);

  if (crawler.init()) {
    crawler.start();
  }

  event_base_dispatch(base);

  return 0;
}

class Uri {
public:
  std::string scheme_;
  std::string host_;
  std::string port_;
  std::string path_;
  std::string query_str_;
};


static std::shared_ptr<Uri> parse_uri(const std::string url) {
  // TODO
  struct evhttp_uri *http_uri = evhttp_uri_parse(url.c_str());
  if (http_uri == nullptr) {
    return nullptr;
  }

  const char *scheme = evhttp_uri_get_scheme(http_uri);
  if ((scheme == NULL) ||
      (strcasecmp(scheme, "http") != 0)) {
    // log("url must be http");
    return nullptr;
  }

  const char *host = evhttp_uri_get_host(http_uri);
  if (host == NULL) {
    // log("url must have a host");
    return nullptr;
  }

  int port = evhttp_uri_get_port(http_uri);
  if (port == -1) {
    port = (strcasecmp(scheme, "http") == 0) ? 80 : 443;
  }

  const char *path = evhttp_uri_get_path(http_uri);
  if (path == NULL || 0x00 == path[0]) {
    path = "/";
  }

  const char *query = evhttp_uri_get_query(http_uri);
  // char uri_str[256];
  std::string query_str;
  if (query == NULL) {
    query_str.assign(path);
    // snprintf(uri_str, sizeof(uri_str) - 1, "%s", path);
  } else {
    // snprintf(uri_str, sizeof(uri_str) - 1, "%s?%s", path, query);
    query_str.assign(path);
    query_str.append("?");
    query_str.append(query);
  }

  std::shared_ptr<Uri> uri(new Uri());

  uri->scheme_ = scheme;
  uri->host_ = host;
  uri->port_ = port;
  uri->path_ = path;
  uri->query_str_ = query_str;

  return uri;
}


Crawler::Crawler(struct event_base *base, const char *url, int thread_num, int max_task_num)
    : base_(base), url_(url), thread_num_(thread_num), max_task_num_(max_task_num)
{
}

bool Crawler::init() {
  std::shared_ptr<Uri> uri = parse_uri(url_);
  if (uri == nullptr) {
    return false;
  }
}


void Crawler::start() {

}
