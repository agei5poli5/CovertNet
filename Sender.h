#include mongoose.h
#ifndef SENDER_H
#define SENDER_H

class Sender{
  Sender(struct http_message* hm);
  char* parseRequest(struct http_message* hm);
  void parseResponse(struct http_message* hm);
}
#endif