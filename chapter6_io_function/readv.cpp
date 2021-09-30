// #include<sys/socket.h>
// #include<netinet/in.h>
// #include<assert.h>
// #include<stdio.h>
// #include<unistd.h>
// #include<stdlib.h>
// #include<string.h>
// #include<errno.h>
// #include<arpa/inet.h>
// #include<sys/sendfile.h>
// #include<fcntl.h>
// #include<string.h>
// #include<sys/stat.h>
// #include<sys/types.h>
// #include<sys/uio.h>

// #define BUFFER_SIZE 1024
//     //两种HTTP状态码和状态信息
// static const char* status_line[2] = {"200 ok","500 Internal server error"};
    
// int main(int argc,char* argv[])
// {

//     //指定的ip地址（此代码中不适用随机的ip地址（INADDR_ANY））
//     const char *ip = argv[1];
//     int port = atoi(argv[2]);
//     //目标文件
//     const char* file_name = argv[3];
//     struct sockaddr_in address;
//     bzero(&address,sizeof(address));
//     address.sin_family = AF_INET;
//     address.sin_port = htons(port);
// // inet_pton用于将点分十进制ip地址转换为网络字节序ip地址且存入dst内存中
//     inet_pton(AF_INET,ip,&address.sin_addr);

//     int sock = socket(PF_INET,SOCK_STREAM,0);//生成服务器的socket文件描述符
//     assert(sock>=0);
//     //将socket地址分配给socket文件描述符（命名socket）
//     int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
//     assert(sock!=-1);
//     ret = listen(sock,5);
//     assert(ret!=-1);

//     struct sockaddr_in clnt;
//     socklen_t clnt_size = sizeof(clnt);
//     int connfd = accept(sock,(struct sockaddr*)&clnt,&clnt_size);
//     if(connfd<0){
//         printf("errno is :%d\n",errno);
//     }
//     else{
//         //获取连接成功
//         //开辟内存空间用于保存状态行，头部字段，空行
//         char head_buf[BUFFER_SIZE];
//         memset(head_buf,'\0',sizeof(head_buf));
//         //用于存放要获取的目标文件内容的应用程序缓存
//         char * file_buf;
//         //获取文件属性：(是否为目录/文件大小等属性)
//         struct stat file_status;
//         //记录文件是否有效
//         bool valid = true;
//         //缓存区head_buf用了多少字节空间
//         int len = 0;
//         if(stat(file_name,&file_status)<0){//返回-1目标文件不存在
//             valid = false;
//         }
//         else{
//             //查看是否为目录
//             if(S_ISDIR(file_status.st_mode)){
//                 valid =false;
//             }
//             else if(file_status.st_mode&S_IROTH){//如果不是目录且当前用户有读取目标文件的权限
//                 //动态分配缓存区file_buf，指定大小为目标文件的大小+1，将内容读入file_buf
//                 int fd = open(file_name,O_RDONLY);
//                 file_buf = new char[file_status.st_size+1];
//                 memset(file_buf,'\0',file_status.st_size+1);
//                 if(read(fd,file_buf,file_status.st_size)<0){
//                     valid = false;
//                 }
//             }
//             else{
//                 valid = false;
//             }

//         }
//         //如果目标文件有效，则发送HTTP应答
//         if(valid){
//             //依次将head的三个部分加入header_buf
//             ret = snprintf(head_buf,BUFFER_SIZE-1,"%s %s\r\n","HTTP/1.1",status_line[0]);
//             len+=ret;
//             ret = snprintf(head_buf+len,BUFFER_SIZE-1-len,"Content-Length:%d\r\n",file_status.st_size);
//             len+=ret;
//             ret = snprintf(head_buf+len,BUFFER_SIZE-1-len,"%s","\r\n");
//             //使用writev将header_buf和file_buf的内容一起写出：
//             struct iovec iv[2];
//             iv[0].iov_base = head_buf;
//             iv[0].iov_len = strlen(head_buf);
//             iv[1].iov_base = file_buf;
//             iv[1].iov_len = file_status.st_size;
//             ret = writev(connfd,iv,2);
//         }
//         else{
//             //文件无效
//             ret = snprintf(head_buf,BUFFER_SIZE-1,"%s %s\r\n","HTTP/1.1",status_line[1]);
//             len+=ret;
//             ret = snprintf(head_buf+len,BUFFER_SIZE-1-len,"%s","\r\n");
//             send(connfd,head_buf,strlen(head_buf),0);

//         }
//         close(connfd);
//         delete [] file_buf;
//     }   
//     close(sock);
//     return 0;
// }