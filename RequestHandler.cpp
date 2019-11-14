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
#include "RequestHandler.h"
#define PORT 8080

  RequestHandler::RequestHandler(unsigned long int serverip, unsigned short int serverport){
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = serverip;
    serveraddr.sin_port = serverport;
    
    proxyaddr.sin_family = AF_INET;
    proxyaddr.sin_addr.s_addr = INADDR_ANY;
    proxyaddr.sin_port = htons(PORT);

    if ((proxy = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
      perror("socket failed"); 
      exit(EXIT_FAILURE); 
    } 
    if (setsockopt(proxy, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
    { 
      perror("setsockopt"); 
      exit(EXIT_FAILURE); 
    }
    if(bind(proxy, (struct sockaddr *)&proxyaddr, sizeof(proxyaddr))<0){
      perror("bind failed");
      exit(EXIT_FAILURE);
    }
    while(true){
      if(listen(proxy, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
      }
      if((client = accept(proxy, (struct sockaddr *)&proxyaddr, (socklen_t *)&addrlen)) 		< 0)
      {
        perror("accept");
        exit(EXIT_FAILURE);
      }
      valread = read(client, buffer, 1024);
      std::vector<std::string> parsed = parseRequest(buffer);
      char* parsey = "hiya";
      send(client, parsey, strlen(parsey), 0);
      std::vector<std::string> newrequest = createRequest(parsed);
      }
}
   std::vector<std::string> RequestHandler::parseRequest(char request[]){
     std::istringstream iss(request);
     std::istream_iterator<std::string> requestiter = std::istream_iterator<std::string>(iss);
     std::istream_iterator<std::string> end = std::istream_iterator<std::string>();
     std::vector<std::string> results(requestiter, end);
     //std::cout << results[0] + "\n";
     return results;
      }

    std::vector<std::string> RequestHandler::createRequest(std::vector<std::string> parsed) 
    {
       std::vector<std::string> request;
       std::vector<std::string>::iterator it;
       request.insert(it, "POST");
       request.insert(it, "foo");
       std::cout << request.front() + "\n";
       return request;
      }
      

    
       
      
      


	int main(){
          //std::cout << "GET\n";
	  unsigned long a = 1203049;
          unsigned short b = 8080;
	  RequestHandler r(a, b);
          return 0;
        }
     
    

		




















