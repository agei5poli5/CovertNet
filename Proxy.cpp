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
#include <stdio.h>
#include <stdlib.h> 
#include "encrypt.h"

static const char *s_http_port = "8080";
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
static const char* body;
static int sem = 0;
static int rc;
const char *err;
static struct mg_connection *listening;
EVP_CIPHER_CTX en, de;
unsigned int salt[] = {12345, 54321};
unsigned char *key_data = a;
int key_data_len = strlen(key_data);



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
       parsed.insert(it, token);
       r.erase(0, pos + delimiter3.length());
   }
   response_line = parsed[0].c_str();
   std::string h;
   for(int i = 1; i < parsed.size()-1; i++){
    
        h += parsed[i];
        h += "\n";
   }
   h += "Transfer-Encoding:chunked";
   extra_headers = h.c_str();
   int k = parsed.size()-1;
   body = parsed[k].c_str();
  

  
   std::istringstream iss_resp(response_line);
   std::istream_iterator<std::string> r_line = std::istream_iterator<std::string>(iss_resp);
   std::istream_iterator<std::string> end_r = std::istream_iterator<std::string>();
   std::vector<std::string> results_r(r_line, end_r);
   response_code = results_r[1].c_str();

   std::stringstream rs(response_code);
   rs >> rc;
   printf("%i", rc);
   std::cout << listening->recv_mbuf.buf << std::endl;
   mg_send_response_line(listening, rc, extra_headers);
   mg_send_http_chunk(listening, "", 0);
   
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

char* packRequest(struct http_message *hm){
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
    body = packRequest(h);
    std::cout << body << std::endl;
    mg_connect_http(&mgr, ev_handler2, url, "Content-Type: application/x-www-form-urlencoded\r\n", body);
}

static void ev_handler1(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
  
    struct http_message *hm = (struct http_message *) p;
    sendRequest(hm);

    //mg_send_http_chunk(c, "", 0); // Tell the client we're finished
   //mg_send_response_line(c, rc, extra_headers);
    //mg_send_http_chunk(c, body, strlen(body));



  
  }
  
}


int main(void) {
   
   encryption* e = new encryption();
    if (e->aes_init(key_data, key_data_len, (unsigned char *)&salt, &en, &de)) {
    printf("Couldn't initialize AES cipher\n");
    return -1;
  }

  struct mg_bind_opts bind_opts;
  mg_mgr_init(&mgr, NULL);
  
  memset(&bind_opts, 0, sizeof(bind_opts));
  

  bind_opts.ssl_cert = s_ssl_cert;
  bind_opts.ssl_key = s_ssl_key;
  bind_opts.error_string = &err;
  

  //std::cout <<"gotthisfar2"<<std::endl;
  listening = mg_bind(&mgr, "8080", ev_handler1);
  //listen = mg_bind_opt(&mgr, s_http_port, ev_handler1, bind_opts);
  //std::cout <<"gotthisfar3"<<std::endl;
  if (listening == NULL) {
    printf("Failed to create listener: %s\n", err);
    return 1;
  }
  mg_set_protocol_http_websocket(listening);
  

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  while (exit_flag == 0) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
