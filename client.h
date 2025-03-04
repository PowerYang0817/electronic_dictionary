#ifndef _MY_H
#define _MY_H

// 标准输入输出库，提供如 printf、scanf 等函数
#include <stdio.h>

// 标准库，包含了许多通用的函数，如内存分配（malloc、free）、进程控制（exit）等
#include <stdlib.h>

// 时间处理库，用于处理日期和时间，如获取当前时间（time 函数）
#include <time.h>

// 字符串处理库，提供了如 strlen、strcpy、strcmp 等字符串操作函数
#include <string.h>

// Unix 标准库，包含了许多系统调用和文件操作函数，如文件描述符操作（read、write）、进程控制（fork）等
#include <unistd.h>

// 文件控制库，用于文件的打开、关闭、读写等操作的控制，如设置文件的打开模式（open 函数）
#include <fcntl.h>

// 目录操作库，用于目录的读取和操作，如打开目录（opendir）、读取目录项（readdir）等
#include <dirent.h>

// 线程库，用于创建和管理线程，如创建线程（pthread_create）、线程同步（pthread_mutex_lock）等
#include <pthread.h>

// 信号量库，用于线程和进程间的同步，如创建信号量（sem_init）、等待信号量（sem_wait）等
#include <semaphore.h>

// 信号处理库，用于处理系统信号，如捕获信号（signal）、阻塞信号（sigprocmask）等
#include <signal.h>

// 共享内存库，用于进程间共享内存的操作，如创建共享内存段（shmget）、连接共享内存段（shmat）等
#include <sys/shm.h> 

// 消息队列库，用于进程间通过消息队列进行通信，如创建消息队列（msgget）、发送消息（msgsnd）等
#include <sys/msg.h> 

// 套接字库，是网络编程的基础，提供了创建和操作套接字的函数，如创建套接字（socket）、绑定地址（bind）等
#include <sys/socket.h>

// 网络地址相关的定义和操作，包含了如 sockaddr_in 结构体、in_addr_t 类型等网络地址相关的定义
#include <netinet/in.h>

// 地址转换库，用于 IP 地址的文本形式和二进制形式之间的转换，如 inet_addr、inet_ntoa 等函数
#include <arpa/inet.h>

//System V IPC（进程间通信）机制的基础头文件，为共享内存、消息队列和信号量提供了公共的键值生成和操作函数。
#include <sys/ipc.h>

//用于处理系统调用和库函数出错时的错误码，通过全局变量 errno 可以获取具体的错误信息，结合 perror 或 strerror 函数可以输出详细的错误描述。
#include <errno.h>

#include <sqlite3.h>
typedef struct{
    int type;//1注册  2登陆 3退出 4查单词  5查历史 6退出登陆
    char user_name[20];
    char user_pwd[20];//密码
    char word[30];//单词
    char word_definition[200];//释义
    int count;//次数
    int user_id;//id
    char date[30];//日期
}msg_t;
typedef struct {
    int user_id;
    char user_name[20];
    int type;
}users;

#endif