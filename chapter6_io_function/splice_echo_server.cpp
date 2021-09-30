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
#include<fcntl.h>

#define BUF_SIZE 1024
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
    //将socket地址分配给socket文件描述符（命名socket）
    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    assert(sock!=-1);
    ret = listen(sock,5);
    assert(ret!=-1);
    //暂停20秒等待客户端连接和异常操作完成
    struct sockaddr_in client;
    socklen_t client_length = sizeof(client);
    int connfd = accept(sock,(struct sockaddr*)&client,&client_length);
    if(connfd<0){
        printf("errno is :%d\n",errno);
    }
    else{
        // char message[50];
        // read(sock,message,sizeof(message)-1);
        // puts(message);
        int pipefd[2];
        assert(ret!=-1);
        //创建管道
        ret =  pipe(pipefd);
        //将connfd上流入的客户数据定向到管道中
        ret = splice(connfd,NULL,pipefd[1],NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
        assert(ret!=-1);
        //将管道的输出重定向到connfd
        ret = splice(pipefd[0],NULL,connfd,NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
        assert(ret!=-1);
        
        close(connfd);

    }
    // msghdr
    close(sock);
    return 0;
//以上代码通过splice函数将客户端的内容读入到管道中，再使用splice函数读出该内容到客户端，实现echo
//不涉及send，recv操作，所以为设计用户空间和内核空间之间的数据拷贝

}