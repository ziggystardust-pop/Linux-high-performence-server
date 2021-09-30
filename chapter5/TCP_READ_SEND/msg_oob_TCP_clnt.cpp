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
int main(int argc,char* argv[]){

                                                                                                                                                                      
    if(argc<=2){
        printf("usage\n");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
// inet_pton用于将点分十进制ip地址转换为网络字节序ip地址且存入dst内存中
    inet_pton(AF_INET,ip,&address.sin_addr);

    int sock = socket(PF_INET,SOCK_STREAM,0);//生成服务器的socket文件描述符
    assert(sock>=0);
 
    if(connect(sock,(struct sockaddr*)&address,sizeof(address))<0){
        printf("connection failed\n");

    }
    else{
        // MSG_OOB发送或接收紧急数据
        const char* oob_data = "abc";
        const char* normal_data = "123";
        send(sock,normal_data,strlen(normal_data),0);
        send(sock,oob_data,strlen(oob_data),MSG_OOB);
        send(sock,normal_data,strlen(normal_data),0);
    }
    close(sock);
    return 0;
}
