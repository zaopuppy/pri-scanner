#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string>
#include <unistd.h>

#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/http.h>
#include <event2/dns.h>


#define log(_format, ...)   printf(__FILE__ ":%d|" _format "\n", __LINE__, ##__VA_ARGS__)

class Uri {
public:
  std::string scheme_;
  std::string host_;
  int port_;
  std::string path_;
  std::string query_;
  std::string uri_str_;
};


static void http_callback(struct evhttp_request *req, void *ctx)
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
}


bool parse_uri(struct evhttp_uri *http_uri, Uri &uri_data)
{
  const char *scheme = evhttp_uri_get_scheme(http_uri);
  if ((scheme == NULL) ||
      (strcasecmp(scheme, "http") != 0)) {
    log("url must be http or https");
    return false;
  }

  const char *host = evhttp_uri_get_host(http_uri);
  if (host == NULL) {
    log("url must have a host");
    return false;
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
  char uri_str[256];
  if (query == NULL) {
    snprintf(uri_str, sizeof(uri_str) - 1, "%s", path);
  } else {
    snprintf(uri_str, sizeof(uri_str) - 1, "%s?%s", path, query);
  }

  uri_data.scheme_ = scheme;
  uri_data.host_ = host;
  uri_data.port_ = port;
  uri_data.path_ = path;
  uri_data.query_ = query == NULL ? "" : query;
  uri_data.uri_str_ = uri_str;

  return true;
}

static void
dns_logfn(int is_warn, const char *msg)
{
  fprintf(stderr, "%s: %s\n", is_warn ? "WARN":"INFO", msg);
}

int get(event_base *base, const char *url, char *buf, int buf_len)
{
  if (NULL == url || buf_len < 0) {
    log("bad parameters");
    return -1;
  }

  if (buf_len > 0 && NULL == buf) {
    log("bad buffer parameter");
    return -1;
  }

  struct evhttp_uri *http_uri = evhttp_uri_parse(url);
  if (NULL == http_uri) {
    log("failed to parse url, check if it's correct");
    return -1;
  }

  Uri uri;
  if (!parse_uri(http_uri, uri)) {
    log("failed to check uri");
    return -1;
  }

  log("scheme: [%s], host: [%s], port: [%d], path: [%s], query: [%s], uri_str: [%s]",
      uri.scheme_.c_str(),
      uri.host_.c_str(),
      uri.port_,
      uri.path_.c_str(),
      uri.query_.c_str(),
      uri.uri_str_.c_str());

  // struct bufferevent *buffer_ev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
  // if (NULL == buffer_ev) {
  //   return -1;
  // }
  // struct evhttp_connection *conn =
  //   evhttp_connection_base_bufferevent_new(base, NULL, buffer_ev,
  //                                          uri.host_.c_str(), uri.port_);
  // if (NULL == conn) {
  //   return -1;
  // }

  // TODO: use dnsbase to do the asynchronized DNS resolving
  // struct evhttp_connection *evhttp_connection_base_new(
  // struct event_base *base, struct evdns_base *dnsbase,
  // const char *address, unsigned short port);
  struct evdns_base *dns_base = evdns_base_new(base, 0);
  evdns_set_log_fn(dns_logfn);
  struct evhttp_connection *conn =
    evhttp_connection_base_new(base, dns_base, uri.host_.c_str(), uri.port_);
  struct bufferevent *buffer_ev = evhttp_connection_get_bufferevent(conn);
  
  struct evhttp_request *req = evhttp_request_new(http_callback, buffer_ev);
  if (NULL == req) {
    return -1;
  }

  struct evkeyvalq *output_headers = evhttp_request_get_output_headers(req);
  evhttp_add_header(output_headers, "Host", uri.host_.c_str());
  evhttp_add_header(output_headers, "Connection", "close");
  
  if (buf_len > 0) {
    struct evbuffer *output_buffer = evhttp_request_get_output_buffer(req);
    evbuffer_add(output_buffer, buf, buf_len);
    char num_buf[16];
    snprintf(num_buf, sizeof(num_buf), "%d", buf_len);
    evhttp_add_header(output_headers, "Content-Length", num_buf);
  }

  int rv = evhttp_make_request(conn, req,
                               buf_len > 0 ? EVHTTP_REQ_POST : EVHTTP_REQ_GET,
                               uri.uri_str_.c_str());
  if (rv != 0) {
    log("failed make request: %d", rv);
    return -1;
  }

  log("waiting");
  event_base_dispatch(base);
  evhttp_connection_free(conn);

  return 0;
}

static void ev_log_callback(int severity, const char *msg)
{
  log("%d|%s", severity, msg);
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    log("bad arguments");
    return -1;
  }

  const char *url = argv[1];
  char buf[1024];

  static struct event_base *base = event_base_new();
  if (NULL == base) {
    log("failed to new event base");
    return -1;
  }

  event_set_log_callback(ev_log_callback);

  get(base, url, NULL, 0);

  event_base_free(base);

  sleep(5);

  return 0;
}


// END
