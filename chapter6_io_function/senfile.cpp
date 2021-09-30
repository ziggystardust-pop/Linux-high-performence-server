#include<sys/socket.h>
#include<netinet/in.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<arpa/inet.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
int main(int argc,char* argv[])
{   
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    const char* file_name = argv[3];
    int filefd = open(file_name,O_RDONLY);
    assert(filefd>0);
    struct stat stat_buf;
    //fstat函数用来将参数fildes 所指的文件状态, 复制到参数buf 所指的结构中(struct stat). Fstat()与stat()作用完全相同, 不同处在于传入的参数为已打开的文件描述词
    fstat(filefd,&stat_buf);
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
        sendfile(connfd,filefd,NULL,stat_buf.st_size);
        close(connfd);
    }
    close(sock);
    return 0;
    
}