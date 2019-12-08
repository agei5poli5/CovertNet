#include "mongoose.h"
#include <iostream>



static const char *s_http_port = "8080";
static int exit_flag = 0;
struct mg_mgr mgr;


static void ev_handler(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {

    struct http_message *hm = (struct http_message *) p;
    
     
    mg_send_head(c, 200, hm->message.len, "Content-Type: text/plain");

    std::cout << "Go gross!" << std::endl; 
    
  
  }
}













int main(void) {
  struct mg_connection *listen;
  mg_mgr_init(&mgr, NULL);
  listen = mg_bind(&mgr, s_http_port, ev_handler);
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