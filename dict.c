#include "server.h"
int log_on(int count,int newSockfd,int *is_login){//0 1 
    msg_t buf={0};
    int ret=recv(newSockfd,&buf,sizeof(buf),0);//包括用户名和密码
    char **resultp;//返回值
    int row,column;//行列
    char sql[100]={0};
    char * errmsg;
    sprintf(sql,"select * from users where user_name='%s' and pwd='%s';",buf.user_name,buf.user_pwd);
    ret=sqlite3_get_table(db,sql,&resultp,&row,&column,&errmsg);
    if(ret==-1){perror("sql err");exit(-1);}
    printf("测试%d",row);//测试
    if(row==0){
        buf.type=0;
        send(newSockfd,&buf,sizeof(buf),0);//登陆失败
        if(count==3){//三次错误机会
            close(newSockfd);//断开连接
            return 0;
        }
        return log_on(count+1,newSockfd,is_login);//再次登陆
    }
    buf.type=1;
    buf.user_id=atoi(resultp[4]);
    printf("当前用户id:%d",buf.user_id);//测试
    send(newSockfd,&buf,sizeof(buf),0);//登陆成功
    puts(resultp[5]);
    *is_login=1;
    return 1;
}

void selectWord(int newSockfd){//查单词
    msg_t buf={0};
    recv(newSockfd,&buf,sizeof(buf),0);
    char word[30];
    strcpy(word,buf.word);
    puts(word);
    char **resultp;//返回值
    int row,column;//行列
    char sql[300]={0};
    sprintf(sql,"select dict.word,dict.word_definition,word_history.view_count \
        from dict left join word_history \
        on dict.word=word_history.word where dict.word ='%s';",word );
    char * errmsg;
    int ret=sqlite3_get_table(db,sql,&resultp,&row,&column,&errmsg);
    if(ret==-1){perror("sql err");exit(-1);}
    if(row==0){
        buf.type=0;
        send(newSockfd,&buf,sizeof(buf),0);//单词不存在
        return;
    }
    buf.type=1;
    strcpy(buf.word,resultp[3]);
    strcpy(buf.word_definition,resultp[4]);
    send(newSockfd,&buf,sizeof(buf),0);
    //添加历史记录
    time_t t=time(NULL);
    char *tm=ctime(&t);
    sprintf(sql,"insert into word_history values (%d,'%s','%s',%d);",buf.user_id,word,tm,0);
    puts(sql);
    ret=sqlite3_exec(db,sql,NULL,NULL,&errmsg);
    if(ret==-1){perror("sql err");exit(-1);}
    //更新次数
    // sprintf(sql,"update word_history set view_count=view_count+1 where word='%s';",word);
    //puts(sql);
}
void selectDistoryWord(int newSockfd){//历史记录
    msg_t buf={0};
    recv(newSockfd,&buf,sizeof(buf),0);
    printf("测试历史数据id:%d",buf.user_id);
    char **resultp;//返回值
    int row,column;//行列
    char sql[300]={0};
    sprintf(sql,"select word_history.word,dict.word_definition,word_history.time \
    from dict join word_history on word_history.word=dict.word\
    where word_history.user_id=%d;",buf.user_id);
    puts(sql);
    char * errmsg;
    int ret=sqlite3_get_table(db,sql,&resultp,&row,&column,&errmsg);
    if(ret==-1){perror("sql err");exit(-1);}
    int i=1;
    buf.type=1;
    while(row--){
        strcpy(buf.word,resultp[3*i]);
        strcpy(buf.word_definition,resultp[1+3*i]);
        strcpy(buf.date,resultp[2+3*i]);
        i++;
        send(newSockfd,&buf,sizeof(buf),0);
    }
    buf.type=0;
    send(newSockfd,&buf,sizeof(buf),0);
}
void registerUser(int newSockfd){//注册
    msg_t buf={0};
    recv(newSockfd,&buf,sizeof(buf),0);//包括用户名和密码
    char sql[200];
    sprintf(sql, "select * from users where user_name = '%s';", buf.user_name);
    char *err_msg = 0;
    char **resultp;
    int row, column;
    int ret = sqlite3_get_table(db, sql, &resultp, &row, &column, &err_msg);
    if (ret == -1) {
        perror("sql err");
        exit(-1);
    }
    if (row == 0) {//未重名
        buf.type=2;
        send(newSockfd,&buf,sizeof(buf),0);
        sprintf(sql, "insert into users (user_name,pwd,type) values ('%s', '%s',1);", buf.user_name, buf.user_pwd);
        ret = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    }else{//重名
        puts("用户名已存在");
        buf.type=0;
        send(newSockfd,&buf,sizeof(buf),0);
        return;
    }
}
void selectWordByDate(){//日期查找

}