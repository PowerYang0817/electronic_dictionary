#include "server.h"
sqlite3 *db;
void * fun_thread(void *p){
    int is_logon = 0;
    int newSoctfd=*(int*)p;
    msg_t buf={0};
    while(1){
        if(is_logon==0){//未登陆
            int ret=recv(newSoctfd,&buf,sizeof(buf),0);//接收消息
            if(ret<=0){pthread_exit(NULL);}
            int type=buf.type;
            if(type==1){
                log_on(0,newSoctfd,&is_logon);//返回值
            }else if(type==2){//注册
                registerUser(newSoctfd);
                //break;
            }
        }else{//已登陆
            int ret=recv(newSoctfd,&buf,sizeof(buf),0);//接收消息
            if(ret<=0){pthread_exit(NULL);}
            int type=buf.type;
            printf("type:%d\n",type);
            if(type==1){//查单词
                selectWord(newSoctfd);
            }else if(type==2){//查历史记录
                selectDistoryWord(newSoctfd);
            }else if(type==3){
                puts("退出登陆");
                is_logon=0;
            }else if(type==4){
                pthread_exit(NULL);
            }

        }
    }
    return 0;
}
int main(int argc, char const *argv[])
{
    //先运行create.sql文件
    int ret=sqlite3_open("./dict.db",&db);
    if(ret==-1){perror("open");exit(-1);}//容错
    int sockfd=socket(AF_INET,SOCK_STREAM,0);//TCP
    struct sockaddr_in myaddr={0};
    myaddr.sin_family=AF_INET;
    myaddr.sin_port=htons(8899);
    myaddr.sin_addr.s_addr=INADDR_ANY;//ip
    //绑定
    int ret2=bind(sockfd,(struct sockaddr*)&myaddr,sizeof(myaddr));
    if(ret2==-1){
        perror("bind");
        exit(-1);
    }
    int ret3=listen(sockfd,5);//设置最大数量
    if(ret3==-1){
        perror("listen");
    }
    while(1){
        struct sockaddr_in youraddr;
        int len=sizeof(youraddr);
        int *newSockfd=(int*)malloc(sizeof(int));//分配独立内存
        *newSockfd=accept(sockfd,(struct sockaddr*)&youraddr,&len);
        puts("连接");
        pthread_t id1;
        int ret4=pthread_create(&id1,NULL,fun_thread,newSockfd); 
        if (ret4 != 0) {
            perror("pthread_create");
            close(*newSockfd);
            free(newSockfd);
            continue;
        }
        pthread_detach(id1);//分离线程，自动释放
    }
    close(sockfd);
    sqlite3_close(db);
    return 0;
}
