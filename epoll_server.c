#include "server.h"
sqlite3 *db;

#define MAX_EVENTS 10 // 最大事件数
// #define BUFFER_SIZE 1024 // 缓冲区大小

// 处理客户端消息
void handle_client_message(int epfd, int fd, int *is_logon_array) {//处理客户端消息，
    msg_t buf = {0};
    int ret = recv(fd, &buf, sizeof(buf), 0);
    if (ret <= 0) {
        // 客户端关闭连接或发生错误
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = fd;
        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);//从epoll中删除,fd从epoll中删除,ev是要删除的事件
        close(fd);
        is_logon_array[fd] = 0; // 重置登录状态 !!!注意
        return;
    }
    int *is_logon = &is_logon_array[fd];
    int type = buf.type;
    if (*is_logon == 0) {
        // 未登陆
        if (type == 1) {
            log_on(0, fd, is_logon);
        } else if (type == 2) {
            // 注册
            registerUser(fd);
        }
    } else {
        // 已登陆
        if (type == 1) {
            // 查单词
            selectWord(fd);
        } else if (type == 2) {
            // 查历史记录
            selectDistoryWord(fd);
        } else if (type == 3) {
            puts("退出登陆");
            *is_logon = 0;
        } else if (type == 4) {
            struct epoll_event ev;
            ev.events = EPOLLIN;
            ev.data.fd = fd;
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
            close(fd);
        }
    }
}

int main(int argc, char const *argv[]) {
    // 先运行create.sql文件
    int ret = sqlite3_open("./dict.db", &db);
    if (ret != SQLITE_OK) {
        perror("open");
        exit(-1);
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP
    if (sockfd == -1) {
        perror("socket");
        sqlite3_close(db);
        exit(-1);
    }
    struct sockaddr_in myaddr = {0};
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(8899);
    myaddr.sin_addr.s_addr = INADDR_ANY; // ip
    // 绑定
    int ret2 = bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr));
    if (ret2 == -1) {
        perror("bind");
        close(sockfd);
        sqlite3_close(db);
        exit(-1);
    }

    int ret3 = listen(sockfd, 5); // 设置最大数量
    if (ret3 == -1) {
        perror("listen");
        close(sockfd);
        sqlite3_close(db);
        exit(-1);
    }

    // 创建 epoll 实例
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        close(sockfd);
        sqlite3_close(db);
        exit(-1);
    }

    // 将监听套接字添加到 epoll 实例中
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        perror("epoll_ctl: listen_fd");
        close(sockfd);
        close(epfd);
        sqlite3_close(db);
        exit(-1);
    }

    // 存储每个客户端的登录状态  使用epoll时，需要使用数组来存储每个客户端的登录状态
    int *is_logon_array = (int *)calloc(FD_SETSIZE, sizeof(int));//个数(最大描述符)+大小
    if (is_logon_array == NULL) {
        perror("calloc");
        close(sockfd);
        close(epfd);
        sqlite3_close(db);
        exit(-1);
    }

    while (1) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);// 阻塞，返回就绪的描述符数量
        //events 是一个结构体数组，用于存储就绪的事件
        if (nfds == -1) {
            perror("epoll_wait");
            continue;
        }

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == sockfd) {
                // 处理新的连接
                struct sockaddr_in youraddr;
                socklen_t len = sizeof(youraddr);
                int newSockfd = accept(sockfd, (struct sockaddr *)&youraddr, &len);
                if (newSockfd == -1) {
                    perror("accept");
                    continue;
                }
                puts("连接");

                // 将新的连接添加到 epoll 实例中
                ev.events = EPOLLIN;// 可读事件
                ev.data.fd = newSockfd;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, newSockfd, &ev) == -1) {
                    perror("epoll_ctl: conn_fd");
                    close(newSockfd);
                }
            } else {
                // 处理已连接的客户端的读写事件
                int fd = events[i].data.fd;// 获取客户端的文件描述符，就是
                handle_client_message(epfd, fd, is_logon_array);// 处理客户端消息
            }
        }
    }

    // 释放资源
    free(is_logon_array);
    close(sockfd);
    close(epfd);
    sqlite3_close(db);
    return 0;
}