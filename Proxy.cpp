#include <string.h>
#include <string>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
//#include "mongoose.c"
#include "mongoose.h"



static const char *s_http_port = "8080";
static const char *url = "10.10.53.67:8080/helloservlet/sayhello";
static int exit_flag = 0;
struct mg_mgr mgr;

static void ev_handler2(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REPLY) {
    struct http_message *hm = (struct http_message *)p;
    c->flags |= MG_F_CLOSE_IMMEDIATELY;
    fwrite(hm->message.p, 1, (int)hm->message.len, stdout);
    //printf("Hello\n");
    putchar('\n');
    exit_flag = 1;
  } else if (ev == MG_EV_CLOSE) {
    exit_flag = 1;
  };
}

void parseRequest(struct http_message *hm){
	const char* req = hm->method.p;
  std::istringstream iss(req);
  std::istream_iterator<std::string> request = std::istream_iterator<std::string>(iss);
  std::istream_iterator<std::string> end = std::istream_iterator<std::string>();
  std::vector<std::string> results(request, end);
  for(int i=0; i<results.size(); i++){
    std::cout << results[i] + "\n";
  }

}
void sendRequest(struct http_message *hm){
    mg_connect_http(&mgr, ev_handler2, url, "Content-Type: application/x-www-form-urlencoded\r\n", "var_1=value_1&var_2=value_2");
}

static void ev_handler1(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    struct http_message *hm = (struct http_message *) p;
    parseRequest(hm);
    sendRequest(hm);
    mg_send_head(c, 200, hm->message.len, "Content-Type: text/plain");
  
  }
  
}


int main(void) {
  struct mg_connection *listen;
  mg_mgr_init(&mgr, NULL);
  listen = mg_bind(&mgr, s_http_port, ev_handler1);
  mg_set_protocol_http_websocket(listen);
  

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  while (exit_flag == 0) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
