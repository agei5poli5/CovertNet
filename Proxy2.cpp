#include <string.h>
#include <string>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
//#include "mongoose.c"
#include "mongoose.h"
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h> 
#include "encrypto.h"

static const char *s_http_port = "443";
static const char *url = "10.10.53.67:8080/helloservlet/sayhello";
static int exit_flag = 0;
struct mg_mgr mgr;
char hidden[1024];
char resp[1024];
static const char *s_ssl_cert = "covertnet_org.crt";
static const char *s_ssl_key = "server.key";
static const char* response_line;
static const char* response_code;
static const char* extra_headers;
static const char* bodyy;
static int sem = 0;
static int rc;
const char *err;
static struct mg_connection* cv_listen;
encrypto* e = new encrypto();
unsigned char *key;
unsigned char *iv;
struct http_message *c_request;
static int waiter = 1;
   




void parseResponse(struct http_message *hm){
  const char* resp = hm->message.p;
  
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
  c = 0;
  std::vector<std::string> parsed = std::vector<std::string>();
  auto it = parsed.begin();
   for(iter = full.begin(); iter != full.end(); iter++){
    std::string field = *iter;
    //std::cout << field << std::endl;
    c++;
    while((pos = field.find(delimiter3)) != std::string::npos){
      token = field.substr(pos + delimiter3.length(), field.length());
      //std::cout << token << std::endl;
      parsed.insert(it, token);
      it = parsed.begin();
      it += c;
      field.erase(0, pos + delimiter3.length());
    }   
   }
   while((pos = r.find(delimiter3)) != std::string::npos){
       token = r.substr(pos + delimiter3.length(), r.length());
       //std::cout << token << std::endl;
       parsed.insert(it, token);
       r.erase(0, pos + delimiter3.length());
   }
   response_line = parsed[0].c_str();
   std::string h;
   for(int i = 1; i < parsed.size()-1; i++){
      
        h += parsed[i];
        h += "\n";
   }
   // h += "Transfer-Encoding:chunked";
   extra_headers = h.c_str();
   int k = parsed.size()-1;
   bodyy = parsed[k].c_str();
  

  
   std::istringstream iss_resp(response_line);
   std::istream_iterator<std::string> r_line = std::istream_iterator<std::string>(iss_resp);
   std::istream_iterator<std::string> end_r = std::istream_iterator<std::string>();
   std::vector<std::string> results_r(r_line, end_r);
   response_code = results_r[1].c_str();
    

    std::cout << response_code << std::endl;
    std::cout << extra_headers << std::endl;
    //std::cout << bodyy << std::endl;
   //std::stringstream rs(response_code);
   //rs >> rc;
   //printf("%i", rc);
   //std::cout << listening->recv_mbuf.buf << std::endl;
   //mg_send_response_line(listening, rc, extra_headers);
   //mg_send_http_chunk(listening, "", 0);
   
  //std::cout << r << std::endl;
}
static void ev_handler3(struct mg_connection *c, int ev, void *p){
    if(ev == MG_EV_HTTP_REPLY){
      std::cout << "Got a response" << std::endl;
         struct http_message *hm = (struct http_message *)p;
         /*if(hm->method.p == "HTTP/1.1 200 OK"){
             key = hm->body.p;
             sendRequest(c_request);
         }*/
    }
}
static void ev_handler2(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REPLY) {
    waiter = 0;
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

  const char* packRequest(struct http_message *hm){
	const char* req = hm->method.p;
  std::istringstream iss(req);
  std::istream_iterator<std::string> request = std::istream_iterator<std::string>(iss);
  std::istream_iterator<std::string> end = std::istream_iterator<std::string>();
  std::vector<std::string> results(request, end);
  std::vector<std::string> encrypted = std::vector<std::string>();
  //unsigned char* ciphertext;
  //auto it = encrypted.begin();
  /*
  for(int i = 0; i < results.size(); i){
    unsigned char* pt = (unsigned char*)results[i].c_str();
    int ciphertext_len = e->encrypt (pt, strlen ((char *)pt), key, iv, ciphertext);
    std::string ct((char*)ciphertext);
    encrypted.insert(it, ct);
    it++;
  }*/
  std::string cv_body("y1=");
  std::string space(" ");
  for(int j = 0; j < 3; j++){
     cv_body += results[j];
     cv_body += space;
     
     //std::string hide(hidden);
     //cv_body += hide;
  }

  for(int j = 3; j < results.size()-1; j+=2){
     snprintf(hidden, sizeof(hidden), "y%d=%s%s", j-1, results[j].c_str(), results[j+1].c_str());
     std::string hide(hidden);
     cv_body += hide;
  }
  const char* returny = cv_body.c_str();
  printf("%s\n", returny);
  return returny;
}

void sendRequest(struct http_message *h){

    const char* body;
    body = packRequest(h);
    printf("%s\n", body);
    //mg_connect_http(&mgr, ev_handler2, url, "Content-Type: application/x-www-form-urlencoded\r\n", body);
}

static void ev_handler1(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
  
    struct http_message *hm = (struct http_message *) p;
    sendRequest(hm);
    //while(waiter == 1){

    //}
    mg_send_response_line(c, 200, "Der Yidisher Yankee");


  
  }
  
}


int main(void) {
   

  struct mg_bind_opts bind_opts;
  mg_mgr_init(&mgr, NULL);
  
  memset(&bind_opts, 0, sizeof(bind_opts));
 

  bind_opts.ssl_cert = s_ssl_cert;
  bind_opts.ssl_key = s_ssl_key;
  bind_opts.error_string = &err;

  //cv_listen = mg_bind(&mgr, s_http_port, ev_handler1);
  
  cv_listen = mg_bind_opt(&mgr, s_http_port, ev_handler1, bind_opts);
  
  if (cv_listen == NULL) {
    printf("Failed to create listener: %s\n", err);
    return 1;
  }
  mg_set_protocol_http_websocket(cv_listen);
  

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  while (exit_flag == 0) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}