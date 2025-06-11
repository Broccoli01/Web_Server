#ifndef CHATSERVER_H
#define CHATSERVER_H

/**
 * @file    ChatServer.h
 * @brief   聊天服务器类定义，基于 Muduo 网络库封装了 TCP 服务端逻辑。
 * @details
 *   该类负责：
 *     1. 启动并监听指定地址/端口的 TCP 连接；
 *     2. 接收客户端连接、断开事件；
 *     3. 收发消息回调处理。
 *
 *   使用前提：
 *     - 需链接 Muduo 库 (muduo_net, muduo_base)；
 *     - 在主线程中创建 EventLoop，并在该循环中调用 start()。
 *
 * @author  xiaojy
 * @date    2025-06-10
 */

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

/**
 * @class ChatServer
 * @brief 基于 Muduo 的简单聊天服务器封装。
 *
 * 负责启动 TCP 服务，接收客户端的连接、断开与消息，并通过回调函数处理业务逻辑。
 */
class ChatServer
{
public:
    /**
     * @brief 构造函数
     * @param loop       事件循环指针，必须保证在其所在线程生命周期内存在
     * @param listenAddr 服务器绑定的 IP 地址和端口
     * @param nameArg    服务器名称（用于日志或内部识别）
     */
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg);

    /**
     * @brief 启动服务器
     * @details
     *   1. 设置连接回调、消息回调；
     *   2. 开启底层 TcpServer 的监听；
     *   3. 由外部调用 loop->loop() 进入事件循环。
     */
    void start();

private:
    /**
     * @brief 新连接或断开时的回调
     * @param conn  描述 TCP 连接的智能指针
     * @note
     *   - 当客户端连接建立时，被调用，可用于欢迎消息或用户管理；
     *   - 当客户端断开时，可释放资源或通知其他用户。
     */
    void onConnection(const TcpConnectionPtr &conn);

    /**
     * @brief 收到消息时的回调
     * @param conn    消息来源的连接
     * @param buffer  数据缓冲区，包含收到的字节流
     * @param time    接收数据的时间戳
     * @note
     *   可在此对 buffer 中的数据进行解析（如消息分包、协议解析），
     *   并将处理结果发送给客户端或广播给其他用户。
     */
    void onMessage(const TcpConnectionPtr &conn,
                   Buffer *buffer,
                   Timestamp time);

    TcpServer   _server;   ///< Muduo 封装的 TCP 服务端对象
    EventLoop  *_loop;     ///< 事件循环指针，负责 IO 多路复用和定时器调度
};
#endif