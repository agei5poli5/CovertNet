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
static const char *s_ssl_cert = "covertnet_org.crt";
static const char *s_ssl_key = "server.key";
static const char* response_line;
static const char* extra_headers;
static const char* body;
const char *err;

void parseResponse(struct http_message *hm){
  const char* resp = hm->message.p;
  //printf("%s\n", resp);
  std::string r(resp);
  size_t pos = 0;
  std::string token;
  std::string delimiter1 = "ageispolis";
  std::string delimiter2 = "&";
  std::string delimiter3 = "xudtp=";
  std::vector<std::string> headers =  std::vector<std::string>();
  std::vector<std::string> full = std::vector<std::string>();
  std::vector<std::string>::iterator iter = headers.begin();  
  while((pos = r.find(delimiter1)) != std::string::npos){
    token = r.substr(0, pos);
    headers.insert(iter, token);
    r.erase(0, pos + delimiter1.length());  
  }

  std::string header = headers[0];
  iter = full.begin();
  int c = 0;
  //std::cout << header << st-+
  while((pos = header.find(delimiter2)) != std::string::npos){
    c++;
    token = header.substr(0, pos);
    //std::cout << token << std::endl;
    full.insert(iter, token);
    iter = full.begin();
    iter += c;
    header.erase(0, pos + delimiter2.length());
  }
  /*std::string sp("\n");
  full.insert(iter, sp);
  c++;
  iter = full.begin();
  iter += c;*/
  //full.insert(iter, r);
  std::vector<std::string> parsed = std::vector<std::string>();
  auto it = parsed.begin();
   for(iter = full.begin(); iter != full.end(); iter++){
    std::string field = *iter;
    c = 0;
    while((pos = field.find(delimiter3)) != std::string::npos){
      c++;
      token = field.substr(pos + delimiter3.length(), field.length());
      std::cout << token << std::endl;
      parsed.insert(it, token);
      it = parsed.begin();
      it += c;
      field.erase(0, pos + delimiter3.length());
    }   
   }
   response_line = parsed[0].c_str();
   std::string h;
   for(int i = 1; i < parsed.size(); i++){
    std::cout << parsed[i] << std::endl;
        h += parsed[i];
        h += "\n";
   }
   extra_headers = h.c_str();

   //std::cout << response_line << std::endl;
   //std::cout << extra_headers << std::endl;



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
  //snprintf(hidden, sizeof(hidden), "y1=%s %s %s%%20&y2=%s%s%s%%20&y3=%s%s%s&y4=%s%s%s", results[0].c_str(), results[1].c_str(), results[2].c_str(), results[2].c_str(), ":", results[3].c_str(), results[5].c_str(), ":", results[6].c_str(), results[7].c_str(), ":", results[8].c_str());
  snprintf(hidden, sizeof(hidden), "y1=%s %s %s%%20&y2=%s%s%s%%20&y3=%s%s%s&y4=%s%s%s", results[0].c_str(), results[1].c_str(), results[2].c_str(), results[3].c_str(), ":", results[4].c_str(), results[5].c_str(), ":", results[6].c_str(), results[7].c_str(), ":", results[8].c_str());
  return hidden;
}

void sendRequest(struct http_message *h){

    char* body;
    body = parseRequest(h);
    std::cout << body << std::endl;
    mg_connect_http(&mgr, ev_handler2, url, "Content-Type: application/x-www-form-urlencoded\r\n", body);
}

static void ev_handler1(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
  
    struct http_message *hm = (struct http_message *) p;
    sendRequest(hm);
    mg_send_head(c, 200, hm->message.len, "Transfer-Encoding: chunked");
    const char* str = "Uhh, hey?";
    mg_send_http_chunk(c, str, strlen(str));
    mg_send_http_chunk(c, "", 0); // Tell the client we're finished



  
  }
  
}


int main(void) {
  
  struct mg_connection *listen;
  struct mg_bind_opts bind_opts;
  mg_mgr_init(&mgr, NULL);
  
  memset(&bind_opts, 0, sizeof(bind_opts));
  

  bind_opts.ssl_cert = s_ssl_cert;
  bind_opts.ssl_key = s_ssl_key;
  bind_opts.error_string = &err;
  

  //std::cout <<"gotthisfar2"<<std::endl;
  listen = mg_bind(&mgr, "8080", ev_handler1);
  //listen = mg_bind_opt(&mgr, s_http_port, ev_handler1, bind_opts);
  //std::cout <<"gotthisfar3"<<std::endl;
  if (listen == NULL) {
    printf("Failed to create listener: %s\n", err);
    return 1;
  }
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
