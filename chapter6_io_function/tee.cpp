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

int main(int argc,char* argv[])
{
    if(argc!=2){
        printf("usgae:%s <file>\n",argv[0]);
        return 1;
    }   
    int filefd = open(argv[1],O_CREAT|O_WRONLY|O_TRUNC,0666);
    assert(filefd>0);
    int pipefd_stdout[2];
    int ret = pipe(pipefd_stdout);
    assert(ret!=-1);
    int pipefd_file[2];
    ret = pipe(pipefd_file);
    assert(ret!=-1);
    
    //将标准输入的内容输入管道pipefd_stdout
    ret = splice(STDIN_FILENO,NULL,pipefd_stdout[1],NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
    //将pipefd_stdout管道输出复制到pipefd_file管道输入端
    ret = tee(pipefd_stdout[0],pipefd_file[1],32768,SPLICE_F_NONBLOCK);
    //将pipefd_file输出重定向到filefd上，从而将标准输入的内容写入文件
    ret = splice(pipefd_file[0],NULL,filefd,NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
    //将管道pipefd_stdout的输出定向到标准输出
    ret = splice(pipefd_stdout[0],NULL,STDOUT_FILENO,NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
    assert(ret!=-1);
    close(filefd);
    close(pipefd_stdout[0]);
    close(pipefd_stdout[1]);
    close(pipefd_file[0]);
    close(pipefd_file[1]);
    return 0;
}