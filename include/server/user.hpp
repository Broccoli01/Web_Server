#ifndef USER_H
#define USER_H

#include <string>
// 匹配User表的ORM类
class User
{
public:
    User(int id = -1, std::string name = "", std::string password = "", std::string state = "offline")
        : id(id), name(name), password(password), state(state) {}

    void setId(int id) { this->id = id; }
    void setName(std::string &name) { this->name = name; }
    void setPassword(std::string &password) { this->password = password; }
    void setState(std::string &state) { this->state = state; }

    int getId() const { return this->id; }
    std::string getName() const { return this->name; }
    std::string getPassword() const { return this->password; }
    std::string getState() const { return this->state; }

private:
    int id;
    std::string name;
    std::string password;
    std::string state;
};

#endif