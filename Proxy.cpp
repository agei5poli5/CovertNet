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
char hidden[1024];
char resp[1024];

void parseResponse(struct http_message *hm){
  const char* resp = hm->message.p;
  std::string r(resp);
  size_t pos = 0;
  std::string token;
  std::string delimiter1 = "ageispolis";
  std::string delimiter2 = "&";
  std::vector<std::string> headers = std::vector<std::string>();
  std::vector<std::string> full = std::vector<std::string>();
  std::vector<std::string>::iterator iter = headers.begin();
  while((pos = r.find(delimiter1)) != std::string::npos){
    token = r.substr(0, pos);
    headers.insert(iter, token);
    r.erase(0, pos + delimiter1.length());
  }
  std::string header = headers[0];
  iter = full.begin();
  //std::cout << header << std::endl;
  while((pos = header.find(delimiter2)) != std::string::npos){
    token = header.substr(0, pos);
    std::cout << token << std::endl;
    full.insert(iter, token);
    iter++;
    header.erase(0, pos + delimiter2.length());
  }
  std::cout << "done" << std::endl;
  /*
  std::vector<std::string>::iterator it = std::vector<std::string>::iterator(); 
   for(it = full.begin(); it != full.end(); it++){
    std::cout << *it;
    std::cout << '\n';
   }*/
  //std::cout << r << std::endl;
}
static void ev_handler2(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REPLY) {
    struct http_message *hm = (struct http_message *)p;
    c->flags |= MG_F_CLOSE_IMMEDIATELY;
    parseResponse(hm);
    //fwrite(hm->message.p, 1, (int)hm->message.len, stdout);
    putchar('\n');
    exit_flag = 1;
  } else if (ev == MG_EV_CLOSE) {
    exit_flag = 1;
  };
}

char* parseRequest(struct http_message *hm){
	const char* req = hm->method.p;
  std::istringstream iss(req);
  std::istream_iterator<std::string> request = std::istream_iterator<std::string>(iss);
  std::istream_iterator<std::string> end = std::istream_iterator<std::string>();
  std::vector<std::string> results(request, end);
  snprintf(hidden, sizeof(hidden), "y1=%s %s %s%%20&y2=%s%s%%20&y3=%s%s&y4=%s%s", results[0].c_str(), results[1].c_str(), results[2].c_str(), results[3].c_str(), results[4].c_str(), results[5].c_str(), results[6].c_str(), results[7].c_str(), results[8].c_str());
  return hidden;
}

void sendRequest(struct http_message *h){

    char* body;
    body = parseRequest(h);
    printf("%s\n", body);
    mg_connect_http(&mgr, ev_handler2, url, "Content-Type: application/x-www-form-urlencoded\r\n", body);
}

static void ev_handler1(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {

    struct http_message *hm = (struct http_message *) p;
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
