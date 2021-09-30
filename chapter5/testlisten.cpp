#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<signal.h>


static bool stop = false;//用于在while循环中作为循环条件

//SIGTERM信号的处理函数，触发时，将while循环中的条件变为true，使之跳出循环
static void handle_term(int sig){
    stop = true;//static变量用static函数
    //static 除了可以声明静态成员变量，还可以声明静态成员函数。普通成员函数可以访问所有成员
    //（包括成员变量和成员函数），静态成员函数只能访问静态成员。
}

int main(int argc,char* argv[]){
    signal(SIGTERM,handle_term);
    if(argc<=3){
        printf("usage:%s ip_address port backlog\n",basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);
    //atoi()函数将数字格式的字符串转换为整数类型
    int sock = socket(PF_INET,SOCK_STREAM,0);
    assert(sock>=0);
    //创建一个ipv4的socket地址
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    // bzero()函数：将内存（字符串）前n个字节清零
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET,ip,&address.sin_addr);//将用字符串表示的ip地址转换为网络字节序整数表示的ip地址
    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    assert(ret = -1);

    ret = listen(sock,backlog);
    while(!stop){
        sleep(1);
    }
    close(sock);
    return 0;
}


