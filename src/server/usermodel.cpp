#include <iostream>
#include "usermodel.hpp"
#include <mysql/mysql.h>
#include "db.h"

bool UserModel::insert(User &user)
{
    //组装SQL语句
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name,password,state) values('%s','%s','%s')",
            user.getName().c_str(), user.getPassword().c_str(), user.getState().c_str());

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            //获取自增的用户ID
            user.setId(mysql_insert_id(mysql.getconnection()));
            return true;
        }
    }
    return false;
}