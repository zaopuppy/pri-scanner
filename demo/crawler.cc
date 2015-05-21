//
// Created by zhaoyi on 15-5-14.
//

#include "crawler.h"

#include <strings.h>

#include <event2/http.h>
#include <future>
#include <unistd.h>
#include <event2/buffer.h>
#include <queue>


#define log(_format, ...)   printf(__FILE__ ":%d|" _format "\n", __LINE__, ##__VA_ARGS__)


int main(int argc, char *argv[])
{
  struct event_base *base = event_base_new();
  struct evdns_base *dns_base = evdns_base_new(base, 1);
  assert(base != NULL);
  assert(dns_base != NULL);

  Z::Crawler crawler(base, dns_base, "http://www.baidu.com", 4, 1000);

  if (crawler.init()) {
    log("init done");
    crawler.start();
  }

  //std::string str = "abc";
  //std::queue<std::string> queue;
  //queue.push(std::move(str));
  //
  //std::string s;
  //while (true) {
  //  s = std::move(queue.front());
  //  queue.pop();
  //  log("item: [%s]", s.c_str());
  //}

  event_base_dispatch(base);

  log("done!!!");

  //std::thread thread([] {
  //  for (int i = 0; i < 10; ++i) {
  //    log("run");
  //    sleep(1);
  //  }
  //});
  //thread.join();
  // std::future<int> f;

  return 0;
}

namespace Z {



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
  uri->port_ = (unsigned short) port;
  uri->path_ = path;
  uri->query_str_ = query_str;

  return uri;
}


Crawler::Crawler(struct event_base *base,
                 struct evdns_base *dns_base,
                 const char *url,
                 int thread_num,
                 int max_task_num)
    : base_(base),
      dns_base_(dns_base),
      url_(url),
      thread_num_(thread_num),
      max_task_num_(max_task_num)
{
}

bool Crawler::init() {
  uri_ = parse_uri(url_);
  if (uri_ == nullptr) {
    return false;
  }

  return true;
}

typedef void (*http_callback_t)(struct evhttp_request *, void *);

// static void http_callback(struct evhttp_request *req, void *ctx)
static void crawler_callback(struct evhttp_request *req, void *arg)
{
  log("http_callback");
  if (NULL == req) {
    log("error occurred");
    return;
  }

  log("response line: %d",
      evhttp_request_get_response_code(req));
  // evhttp_request_get_response_code_line(req));

  char buf[256];
  for (int rv = evbuffer_remove(evhttp_request_get_input_buffer(req), buf, sizeof(buf)-1);
       rv > 0; rv = evbuffer_remove(evhttp_request_get_input_buffer(req), buf, sizeof(buf)-1)) {
    buf[rv] = 0x00;
    log("data: [%s]", buf);
  }

  log("http_callback done");
}

static void http_get(struct event_base *base,
                     struct evdns_base *dns_base,
                     const std::shared_ptr<Uri> uri,
                     http_callback_t  callback)
{
  struct evhttp_connection *conn =
      evhttp_connection_base_new(
          base, dns_base, uri->host_.c_str(), uri->port_);

  struct evhttp_request *req = evhttp_request_new(callback, nullptr);

  struct evkeyvalq *headers = evhttp_request_get_output_headers(req);
  evhttp_add_header(headers, "Host", uri->host_.c_str());
  evhttp_add_header(headers, "Connection", "close");

  int rv = evhttp_make_request(
      conn, req, EVHTTP_REQ_GET, uri->query_str_.c_str());
  if (rv != 0) {
    log("failed to make request: %d", rv);
    return;
  }

  log("sent");

  // TODO: need timeout in case of never-response server
  // evhttp_connection_free(conn);
}

class Lock {
public:
  Lock(std::mutex &lock): lock_(lock) { lock_.lock(); }
  ~Lock() { lock_.unlock(); }

private:
  std::mutex &lock_;
};

void Crawler::start() {
  std::queue<std::string> url_queue;
  std::mutex url_queue_lock;
  url_queue.push(url_);
  bool stop = false;
  std::string url;
  for (;;) {
    {
      Lock(url_queue_lock);
      if (url_queue.empty()) {
        if (task_running) {
          wait_for_task();
          if (url_queue.empty()) {
            stop = true;
          }
        } else {
          stop = true;
        }
      } else {
        url = std::move(url_queue.front());
        url_queue.pop();
      }
    }
    http_get(base_, dns_base_, uri_, crawler_callback);
  }

  while (queue_is_not_empty) {
    url = get_url_from_queue();
    fetch(url);
  }
}

//class EventLoop: public Thread {
//public:
//
//  virtual bool init() override;
//
//  virtual bool start() override;
//
//  virtual void run() override;
//
//
//  // Future<
//
//private:
//  struct event_base *base_;
//  bool stop_;
//};
//
//bool EventLoop::init() {
//  if (!Thread::init()) {
//    return false;
//  }
//
//  base_ = event_base_new();
//  if (base_ == nullptr) {
//    return false;
//  }
//
//  return true;
//}
//
//bool EventLoop::start() {
//  return Thread::start();
//}
//
//void EventLoop::run() {
//  while (!stop_) {
//    event_base_loop(base_, EVLOOP_ONCE);
//  }
//}


}

