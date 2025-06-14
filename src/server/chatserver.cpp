#include "chatserver.hpp"   // 聊天服务器类的声明，包含 EventLoop、TcpServer 等成员
#include "nlohmann/json.hpp"         // nlohmann::json 库，用于 JSON 解析和序列化
#include "chatservice.hpp"  // 聊天业务逻辑类 ChatService 的定义
#include <functional>       // std::bind, std::placeholders 占位符，用于回调绑定
#include <string>           // std::string 类型

using json = nlohmann::json;  // 将 nlohmann::json 别名为 json，方便使用

// 构造函数：
//  loop       - 事件循环指针，用于驱动网络事件
//  listenAddr - 监听的 IP 地址和端口
//  nameArg    - 服务器实例名称（用于日志标识等）
ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg)  // 利用 TcpServer 构造函数，初始化底层网络服务器
    , _loop(loop)                         // 保存事件循环指针，便于定时任务或跨线程调用
{
    // 当有新连接建立或断开时，调用 ChatServer::onConnection
    _server.setConnectionCallback(
        std::bind(&ChatServer::onConnection,
                  this,
                  std::placeholders::_1 /* TcpConnectionPtr */));

    // 当有数据可读时，调用 ChatServer::onMessage
    _server.setMessageCallback(
        std::bind(&ChatServer::onMessage,
                  this,
                  std::placeholders::_1 /* TcpConnectionPtr */,
                  std::placeholders::_2 /* Buffer* */,
                  std::placeholders::_3 /* Timestamp */));

    // 设置底层服务器的线程数：1 个 IO 线程 + 3 个工作线程，
    // 总共 4 个线程并发处理
    _server.setThreadNum(4);
}

// 启动服务器：绑定监听端口，启动线程池，并开始事件循环
void ChatServer::start()
{
    _server.start();
}

// 连接建立或断开时被调用
//  conn - 当前的 TCP 连接智能指针
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        // 连接断开时，主动关闭底层 socket
        conn->shutdown();
    }
}

// 收到客户端发来的数据时被调用
//  conn   - 当前连接
//  buffer - 接收数据缓冲区
//  time   - 收到数据的时间戳
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buffer,
                           Timestamp time)
{
    // 将缓冲区中的所有数据取出并转换为字符串
    string buf = buffer->retrieveAllAsString();

    // 解析 JSON 报文（例如：{"msgid":1, "data":...}）
    json js = json::parse(buf);

    // 根据 msgid 分发到对应的业务处理函数
    ChatService::instance()->getHandler(js["msgid"].get<int>())(conn, js, time);
}
