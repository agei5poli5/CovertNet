#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <string> 
#include <vector>
#include <cstring>
#include <iostream>
#include <iterator>
#include <sstream>

#define PORT 8080

  class RequestHandler{
    public:
      int proxy, client, valread;
      struct sockaddr_in proxyaddr, serveraddr;
      int opt = 1;
      int addrlen = sizeof(proxyaddr);
      char buffer[1024];
      RequestHandler(unsigned long int, unsigned short int);
      std::vector<std::string> parseRequest(char[]);
      std::vector<std::string> createRequest(std::vector<std::string>);
    };



