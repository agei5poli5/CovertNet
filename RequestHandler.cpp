#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h>
#include <string> 
#include <vector>
#include <cstring>
#include <iostream>
#include <iterator>
#include <sstream>
#include "RequestHandler.h"
#define PORT 8080

  RequestHandler::RequestHandler(const char* serverip, unsigned short int serverport){
    opt = 1;
    addrlen = sizeof(proxyaddr);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(serverport);
    serveraddr.sin_addr.s_addr = inet_addr(serverip);

    
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
      const char* post = "POST /helloservlet/sayhello HTTP/1.1 \nHost: 10.10.53.67:8080 \nUser-Agent:Mozilla/5.0 \nAccept: text/xml,text/html,text/plain,/image/jpeg \nAccept-Language: en-us,en \nAccept-Encoding: gzip, deflate \nAccept-Charset:ISO-8859-1.utf-8 \nKeep-Alive:300 \nConnection:keep-alive";
      if ((serversock = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
      if (setsockopt(serversock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }
      if (bind(serversock, (struct sockaddr *)&serveraddr,  
                                 sizeof(serveraddr))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
      send(serversock, post, strlen(post), 0);  
      
      //std::vector<std::string> newrequest = createRequest(parsed);
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
       std::string url = "foo.php";
       //std::cout << parsed[0];
       std::vector<std::string> request;
       auto it = request.begin();
       request.insert(it, "POST" + url + "HTTP/1.1");
       it = request.begin();
       request.insert(it++, "Host: " + url);
       //std::cout << request.front() + "\n";
       return request;
      }
      

    
       
      
      


	int main(){
          //std::cout << "GET\n";
	  const char* a ="10.10.53.67"; 
          unsigned short b = 8080;
	  RequestHandler r(a, b);
          return 0;
        }
     
    

		




















