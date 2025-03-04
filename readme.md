### 环境要求

- **操作系统**：Linux 或 macOS（建议使用 Ubuntu 18.04 及以上版本）
- **编程语言**：C 语言（需支持 C99 标准）
- **数据库**：SQLite3 数据库

### 安装依赖项

确保你的系统已经安装了 SQLite3 开发库。在 Ubuntu 系统上，可以使用以下命令进行安装：

```bash
sudo apt-get update
sudo apt-get install sqlite3 libsqlite3-dev
```

### 导入数据库内容

1. 运行 `create.sql` 文件创建数据库表结构：

```bash
sqlite3 dict.db < create.sql
```

这将创建一个名为 `dict.db` 的 SQLite 数据库，并根据 `create.sql` 文件中的 SQL 语句创建相应的表。

1. 编译并运行 `insertdata.c` 文件，将数据插入到数据库中：

```bash
gcc insertdata.c -o insertdata -lsqlite3
./insertdata
```

### 编译项目

分别编译服务器和客户端代码：

```bash
gcc server.c dict.c -o server -lsqlite3 -lpthread
gcc client.c -o client
```

## 使用方法

### 启动服务器

在终端中运行以下命令启动服务器：

```bash
./server
```

服务器将开始监听指定的端口，等待客户端连接。

### 启动客户端

在另一个终端中运行以下命令启动客户端：

```bash
./client
```

客户端将连接到服务器，并显示操作菜单。

### 进行操作

- **查询单词**：在客户端输入要查询的单词，客户端将向服务器发送查询请求，服务器查询数据库并返回单词的释义信息。
- **查看历史记录**：在客户端选择查看历史记录选项，客户端将向服务器发送请求，服务器返回用户的查询历史记录。

## 项目结构

```plaintext
electronic_dictionary/
├── create.sql            # 数据库表创建脚本
├── insertdata.c          # 数据插入程序
├── server.c              # 服务器主程序
├── server.h              # 服务器头文件
├── dict.c                # 字典功能实现文件
├── client.c              # 客户端主程序
├── client.h              # 客户端头文件
├── dict.db               # 数据库文件
└── README.md             # 项目说明文件
```

## 贡献指南

### 提交问题

如果使用项目过程中遇到问题或有建议，可以在项目的 GitHub 仓库中提交一个 Issue。请提供详细的问题描述、复现步骤和相关的错误信息。



### 代码贡献

1. Fork 项目到自己的 GitHub 仓库。
2. 创建一个新的分支：`git checkout -b new-feature`。
3. 进行代码修改和测试。
4. 提交代码到自己的仓库：`git push origin new-feature`。
5. 在项目的 GitHub 仓库中创建一个 Pull Request。

### 代码风格规范

请遵循项目的代码风格规范，确保代码的一致性和可读性。