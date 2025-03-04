// #include "my.h"
// int main(int argc, char const *argv[])
// {
//     sqlite3 *db;
//     int ret=sqlite3_open("./dict.db",&db);
//     if(ret==-1){
//         exit(-1);
//     }
//     FILE *fd=fopen("dict.txt","r");
//     if(fd==NULL){exit(-1);}
//     char sql[300];
//     char temp[300];
//     while((fgets(temp,sizeof(temp),fd))>0){//txt文件添加至数据库中
//         char word[50];
//         char *word_c;
//         word_c=strstr(temp," ");
//         size_t spaces=strspn(word_c," "); //连续包含字符集
//         word_c=word_c+spaces;
//         strncpy(word,temp,strcspn(temp," "));//连续不包含字符集
//         // puts(word_c);
//         sprintf(sql,"insert into dict values('%s','%s');",word,word_c);
//         // printf("%d",sqlite3_exec(db,sql,NULL,NULL,NULL));
//         sqlite3_exec(db,sql,NULL,NULL,NULL);
//         memset(temp,0,sizeof(temp));
//     }
//     fclose(fd);
//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

// 去除字符串末尾的换行符
void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main(int argc, char const *argv[]) {
    sqlite3 *db;
    int ret = sqlite3_open("./dict.db", &db);
    if (ret != SQLITE_OK) {
        fprintf(stderr, "无法打开数据库: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    FILE *fd = fopen("dict.txt", "r");
    if (fd == NULL) {
        perror("无法打开文件");
        sqlite3_close(db);
        return -1;
    }

    char temp[300];
    while (fgets(temp, sizeof(temp), fd) != NULL) {
        remove_newline(temp);  // 去除换行符

        char word[30];
        char *word_c;
        word_c = strstr(temp, " ");
        if (word_c == NULL) {
            continue;  // 跳过没有空格的行
        }
        size_t spaces = strspn(word_c, " ");  // 连续包含字符集
        word_c = word_c + spaces;
        strncpy(word, temp, strcspn(temp, " "));//!
        word[strcspn(temp, " ")] = '\0';  // 确保字符串以空字符结尾

        // 使用 sqlite3_mprintf 安全地构建 SQL 语句
        char *sql = sqlite3_mprintf("insert into dict values('%q','%q');", word, word_c);
        if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
            fprintf(stderr, "SQL 执行错误: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_free(sql);  // 释放由 sqlite3_mprintf 分配的内存
    }

    fclose(fd);
    sqlite3_close(db);
    return 0;
}

