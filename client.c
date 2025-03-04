#include "client.h"
int sockfd;
int is_login_c;
users userNow;
void login (){
    char name[30],pwd[30];
    msg_t buf={0};
    buf.type=1;
    send(sockfd,&buf,sizeof(buf),0);//登陆请求
    int n=0;//错误次数
    while(1){
        printf("用户名:");
        scanf("%s",name);
        printf("密码:");
        scanf("%s",pwd);
        strcpy(buf.user_name,name);
        strcpy(buf.user_pwd,pwd);
        send(sockfd,&buf,sizeof(buf),0);//传输
        recv(sockfd,&buf,sizeof(buf),0);
        if(buf.type==1){//成功
            puts("登陆成功");
            strcpy(userNow.user_name,name);
            userNow.user_id=buf.user_id;
            printf("当前用户id:%d\n",buf.user_id);//测试
            is_login_c=1;
            return;
        }else{
            n++;
            if(n==3){
                puts("错误次数过多");
                exit(0);
            }
            puts("用户名或密码错误");
        }
    }
}
void select_word(){
    msg_t buf={0};
    buf.type=1;
    buf.user_id=userNow.user_id;
    strcpy(buf.user_name,userNow.user_name);
    send(sockfd,&buf,sizeof(buf),0);
    printf("请输入单词:");
    scanf("%s",buf.word);
    if(strcmp(buf.word,"#")==0){//
        return;
    }
    send(sockfd,&buf,sizeof(buf),0);
    recv(sockfd,&buf,sizeof(buf),0);
    if(buf.type==1){
        printf("单词:%s\n解释:%s\n ",buf.word,buf.word_definition);
    }else if(buf.type==0){
        printf("单词不存在\n");
    }
}
void select_word_history(){
    msg_t buf={0};
    buf.type=2;
    buf.user_id=userNow.user_id;
    strcpy(buf.user_name,userNow.user_name);
    send(sockfd,&buf,sizeof(buf),0);//申请
    send(sockfd,&buf,sizeof(buf),0);//传id
    int flag=1;
    while(flag){
        recv(sockfd,&buf,sizeof(buf),0);
        if(buf.type==0){
            flag=0;
            break;
        }
        printf("单词:%s\n解释:%s\n时间:%s\n",buf.word,buf.word_definition,buf.date);
    }
}
void quit_log(){//退出登陆
    msg_t buf;
    buf.type=3;
    send(sockfd,&buf,sizeof(buf),0);
}
void register_user(){//注册
    msg_t buf;
    buf.type=2;
    send(sockfd,&buf,sizeof(buf),0);//请求
    printf("请输入用户名:");
    scanf("%s",buf.user_name);
    printf("请输入密码:");
    scanf("%s",buf.user_pwd);
    send(sockfd,&buf,sizeof(buf),0);//传输
    recv(sockfd,&buf,sizeof(buf),0);//接收,判断
    if(buf.type==2){
        puts("注册成功");
    }else{
        puts("注册失败");
    }
}
int main(int argc, char const *argv[])
{   
    is_login_c=0;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serveraddr={0};
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(8899);
    serveraddr.sin_addr.s_addr=INADDR_ANY;//ip
    connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    while(1){
        printf("--------欢迎使用电子词典--------\n");
        printf("-----------------------------\n");
        if(is_login_c==0){
            int n;
            printf("1登陆,2注册 3.退出\n输入选择的功能:");
            scanf("%d",&n);
            if(n==1){//登陆
                login();
            }else if(n==2){//注册
                register_user();
            }else if(n==3){//退出
                exit(0);
            }else{
                printf("参数错误\n");
            }
        }else{
            //页面
            printf("1.查询单词 2.查历史记录 3.退出登陆 4.退出 功能 :");
            int n;
            scanf("%d",&n);
            if(n==1){//查询
                select_word();
            }else if(n==2){//历史
                select_word_history();
            }else if(n==3){//退出登陆
                quit_log();
                is_login_c=0;
            }else if(n==4){//结束程序
                exit(-1);
            }else{//错误
                puts("参数错误");
            }
        }
    }
    close(sockfd);
    return 0;
}
