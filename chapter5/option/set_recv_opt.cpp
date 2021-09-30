#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<signal.h>
#include<errno.h>

int main(){

    // int sock = socket(PF_INET,SOCK_STREAM,0);
    // assert(sock>=0);
    // int reuse = 1;
    // setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    // struct sockaddr_in address;
    // bzero(&address,sizeof(address));
    // address.sin_family = AF_INET;
    // inet_pton(AF_INET,ip,&address.sin_addr);
    // address.sin_port = htons(port);
    // int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));

}