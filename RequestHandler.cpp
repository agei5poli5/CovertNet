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
#define PORT 8080;
class requestHandler {
  int proxy, client, valread;
  struct sockaddr_in proxyaddr, serveraddr;
  int opt = 1;
  int addrlen = sizeof(proxyaddr);
  char buffer[1024] = {0};

  requestHandler(unsigned long serverip, unsigned short serverport){
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
}
}
     
    

		



















}
