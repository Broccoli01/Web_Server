
#ifndef DB_H
#define DB_H

#include <string>
#include <mysql/mysql.h>

class MySQL
{
public:
    // 构造/析构
    MySQL();
    ~MySQL();

    // 连接数据库
    bool connect();

    // 执行增删改操作
    bool update(const std::string &sql);

    // 执行查询操作
    MYSQL_RES *query(const std::string &sql);

    // 获取链接
    MYSQL *getconnection() const;

private:
    MYSQL *_conn; // MySQL 连接句柄
};

#endif // DB_H