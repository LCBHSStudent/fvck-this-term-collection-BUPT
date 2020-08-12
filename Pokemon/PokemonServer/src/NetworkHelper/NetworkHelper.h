#ifndef SOCKETHELPER_H
#define SOCKETHELPER_H

#include <PreCompile.h>
#include "../../protocol/UserProtocol.pb.h"

#define NET_SIG(_name) \
    void sig##_name(QTcpSocket* client, QByteArray data)

class NetworkHelper: public QObject {
    Q_OBJECT
public:
    explicit    /*构造网络辅助类*/
        NetworkHelper(QObject *parent = nullptr);
    virtual     /*析构网络辅助类*/
        ~NetworkHelper();
    
public FUNCTION:
    size_t          /*发送QString封装的数据到Client socket*/
        sendToClient(QTcpSocket*, const QString&);
    size_t          /*发送QByteArray封装的数据到Client socket*/
        sendToClient(QTcpSocket*, QByteArray&&);
    static int      /*利用反射parse protobuf message*/
        DynamicParseFromPBFile(
            const QString&  filename,
            const QString&  classname,
            std::function<
                void(::google::protobuf::Message* msg)
            > cb
        );
    
public RESOURCE:
    // 指定端口号
    inline static int port = 1919;

signals:
    // 读取到客户端消息报文
    NET_SIG(GetMessage);
    // 客户端断开连接
    NET_SIG(UserDisconnected);
	
private slots:
    void    /*处理新加入的客户端连接*/
        slotNewConnection();
    void    /*读取client socket送来的数据*/
        slotReadClient();
    void    /*client断开连接时调用的处理函数*/
        slotGotDisconnection();
    
private RESOURCE:
    QList<QTcpSocket*>      /*存储所有client socket*/
        m_clients {};
    QTcpServer*             /*Qt封装的TcpServer类*/
        m_server = nullptr;
};

#endif // SOCKETHELPER_H
