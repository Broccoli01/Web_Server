#ifndef CHATSERVICE_H
#define CHATSERVICE_H

// ChatService.h
// 聊天服务器业务处理类（单例模式），负责注册各类消息的处理回调并分发调用

#include <unordered_map>             // std::unordered_map
#include <functional>                // std::function
#include <muduo/net/TcpConnection.h> // muduo::net::TcpConnectionPtr
//#include <memory>
#include "nlohmann/json.hpp" // nlohmann::json
#include "usermodel.hpp"
using namespace std;
using namespace std::placeholders;

// 定义消息处理函数类型：
// 参数：
//   conn - 客户端连接指针
//   js   - JSON 对象，承载消息内容
//   time - 消息到达的时间戳
using MsgHandler = function<void(const muduo::net::TcpConnectionPtr &conn,
                                 nlohmann::json &js,
                                 muduo::Timestamp)>;

// ChatService: 聊天服务器的核心业务类，采用单例模式
class ChatService
{
public:
    // 获取单例实例
    // 返回值：指向唯一 ChatService 对象的指针
    static ChatService *instance();

    // 处理登录请求
    // 当收到客户端 LOGIN_MSG 时调用
    void login(const muduo::net::TcpConnectionPtr &conn,
               nlohmann::json &js,
               muduo::Timestamp time);

    // 处理注册请求
    // 当收到客户端 REG_MSG 时调用
    void reg(const muduo::net::TcpConnectionPtr &conn,
             nlohmann::json &js,
             muduo::Timestamp time);

    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);

private:
    // 私有构造函数，防止外部通过 new 创建多个实例
    ChatService();

    // 存储 消息 ID -> 对应的处理函数 映射表
    // 在构造函数中注册各消息类型的处理函数到此 map
    unordered_map<int, MsgHandler> _msgHandlerMap;
    //数据操作类对象
    UserModel _userModel;
};

#endif // CHATSERVICE_H
