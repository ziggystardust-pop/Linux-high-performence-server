#include<sys/socket.h>
#include<netinet/in.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<arpa/inet.h>


int main(int argc,char* argv[]){
    if(argc<=2){
        printf("Usage : %s ip_address port_number\n",basename(argv[0]));
        return 1;
    }
    //指定的ip地址（此代码中不适用随机的ip地址（INADDR_ANY））
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
// inet_pton用于将点分十进制ip地址转换为网络字节序ip地址且存入dst内存中
    inet_pton(AF_INET,ip,&address.sin_addr);

    int sock = socket(PF_INET,SOCK_STREAM,0);//生成服务器的socket文件描述符
    assert(sock>=0);
    //将socket地址分配给socket文件描述符（命名socket）
    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    assert(sock!=-1);
    ret = listen(sock,5);
    assert(ret!=-1);

    struct sockaddr_in clnt;
    socklen_t clnt_size = sizeof(clnt);
    int connfd = accept(sock,(struct sockaddr*)&clnt,&clnt_size);
    if(connfd<0){
        printf("errno is :%d\n",errno);
    }
    else{
        close(STDOUT_FILENO);
        dup(connfd);
        printf("abcd\n");
        close(connfd);

    }
    close(sock);
    return 0;
    
//先关闭标准输出文件描述符，再复制connfd，这样复制出来的文件描述符就是系统
// 当前可用最小值，即为之前的标准输出描述符即为1，这样使用printf输出的内容就会直接发送到
// 与客户端对应的socket上
// 此为CGI服务器的工作原理

}
