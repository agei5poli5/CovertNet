#include mongoose.h

static int exit_flag = 0;
static const char *url = "10.10.53.67:8080/helloservlet/sayhello";

void parseResponse(struct http_message *hm){
  const char* resp = hm->message.p;
  std::string r(resp);
  size_t pos = 0;
  std::string token;
  std::string delimiter1 = "ageispolis";
  std::string delimiter2 = "&";
  std::string delimiter3 = "xudtp=";
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
  int c = 0;
  //std::cout << header << std::endl;
  while((pos = header.find(delimiter2)) != std::string::npos){
    c++;
    token = header.substr(0, pos);
    full.insert(iter, token);
    iter = full.begin();
    iter += c;
    header.erase(0, pos + delimiter2.length());
  }
  
  full.insert(iter, r);
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

static void ev_handler(struct mg_connection *c, int ev, void *p){
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
Sender::Sender(struct http_message *hm){
  struct mg_mgr mgr;

  mg_mgr_init(&mgr, NULL);
   char* body;
   body = parseRequest(hm);
   mg_connect_http(&mgr, ev_handler, url, "Content-Type: application/x-www-form-urlencoded\r\n", body);


  while (exit_flag == 0) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;

}

int main(void){

}