#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<signal.h>

static bool switch_user(uid_t user_id,gid_t gp_id)//gid是组，uid是用户
{
    //确保目标用户不是root
    if((user_id==0)||(gp_id==0)){
        return false;
    }
    //确保当前用户合法：（如果当前用户是root或者已经是目标用户）
    gid_t gid = getgid();
    uid_t uid = getuid();

    if(((gid!=0)&&(uid!=0))||((gid!=gp_id)&(uid!=user_id))){
        return false;
    }
    //如果不是root，则已经是目标用户
    if(uid!=0){
        return true;
    }
    //如果是root则切换
    if((setgid(gp_id)<0)||(setuid(user_id)<0)){
        return false;
    }
    return true;
}
int main()
{
    switch_user(2,2);

    return 0;
}