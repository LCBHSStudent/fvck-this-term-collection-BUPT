#include "NetworkHelper.h"

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>

/**
 * @brief NetworkHelper::NetworkHelper
 *        网络辅助类构造函数
 * @param parent    父QOBJECT指针（default nullptr)
 */
NetworkHelper::NetworkHelper(QObject *parent):
    QObject(parent),
    m_server(new QTcpServer)
{
    if(!m_server->listen(QHostAddress::Any, NetworkHelper::port)) {
        throw std::runtime_error("failed to listen local port");
    } else {
        connect(
            m_server, &QTcpServer::newConnection,
            this, &NetworkHelper::slotNewConnection, Qt::DirectConnection
        );
    }
}
/**
 * @brief NetworkHelper::~NetworkHelper
 *        析构网络辅助类
 */
NetworkHelper::~NetworkHelper() {
    if(!m_server) {
        delete m_server;
        m_server = nullptr;
    }
    for(auto& pClient: m_clients) {
        delete pClient;
        pClient = nullptr;
    }
}

// 记得CLIENT 断开时 DISCONNECT
/**
 * @brief NetworkHelper::slotNewConnection
 *        当有新客户端socket接入时调用的槽函数, 负责加入用户 连接信号
 */
void NetworkHelper::slotNewConnection() {
    QTcpSocket* clntSocket = m_server->nextPendingConnection();
    // 似乎会自动释放啊嗯
    connect(
        clntSocket, &QTcpSocket::disconnected,
        clntSocket, &QTcpSocket::deleteLater
    );
    connect(
        clntSocket, &QTcpSocket::readyRead,
        this, &NetworkHelper::slotReadClient
    );
    connect(
        clntSocket, &QTcpSocket::disconnected,
        this, &NetworkHelper::slotGotDisconnection
    );
    
    qDebug() << "heres incoming a connection";
    
    m_clients << clntSocket;
}

/**
 * @brief NetworkHelper::slotReadClient
 *        阅读client发来的数据报
 */
void NetworkHelper::slotReadClient() {
    QTcpSocket* client = reinterpret_cast<QTcpSocket*>(sender());
//  TODO: 在此处理TCP粘包问题，修改每个数据包前四位为数据长度，再四位为 protobuf MessageType
//  先拿到数据包头的大小?
    while (client->bytesAvailable() >= sizeof(uint32)) {
        char lengthArr[sizeof(uint32)] = {0};
        client->read(lengthArr, sizeof(uint32));
        auto length = *reinterpret_cast<uint32*>(lengthArr);
        if (length < 0) {
            client->readAll();
//            throw std::runtime_error("")
        } else {
            QByteArray data = client->read(length);
            emit sigGetMessage(client, data);
        }
    }
}

/**
 * @brief NetworkHelper::slotGotDisconnection
 *        反馈断开连接信号
 */
void NetworkHelper::slotGotDisconnection() {
    qDebug() << "someone disconnected";
    
    auto client = reinterpret_cast<QTcpSocket*>(sender());
    
    disconnect(
        client, &QTcpSocket::readyRead,
        this, &NetworkHelper::slotReadClient
    );
    disconnect(
        client, &QTcpSocket::disconnected,
        this, &NetworkHelper::slotGotDisconnection
    );
    disconnect(
        client, &QTcpSocket::disconnected,
        client, &QTcpSocket::deleteLater
    );
    
    emit sigUserDisconnected(client, QByteArray());
    
    m_clients.removeAt(
        m_clients.indexOf(client)
    );
    // emit
}

/**
 * @brief NetworkHelper::sendToClient
 *        发送数据报文到指定的客户端
 * @param socket    client的tcpsocket指针
 * @param msg       QString类型封装的数据
 * @return {size_t} 发送报文的长度
 */
size_t NetworkHelper::sendToClient(
    QTcpSocket*     socket,
    const QString&  msg
) {
    return 0;
}

/**
 * @brief NetworkHelper::sendToClient
 *        发送数据报文到指定的客户端
 * @param socket    client的tcpsocket指针
 * @param data      QByteArray类型封装的数据
 * @return {size_t} 发送报文的长度
 */
size_t NetworkHelper::sendToClient(
    QTcpSocket*    socket,
    QByteArray&&   data
) {
    socket->write(data, data.length());
    return data.length();
}

/**
 * @brief NetworkHelper::DynamicParseFromPBFile
 *        利用反射获取protobuf报文结构体信息并生成实例
 * @param filename  pb 文件名
 * @param classname 类名
 * @param cb        command buffer
 * @return {int}    {0: 成功; 1: 未获取到类型; 2: 未获取到信息; 3: 生成信息失败}
 */
int NetworkHelper::DynamicParseFromPBFile(
    const QString& filename,
    const QString& classname,
    std::function<void(::google::protobuf::Message* msg)> cb
) {
    // TODO 检查文件名是否合法
    auto pos = filename.lastIndexOf('/');
    QString path;
    QString file;
    if(pos == std::string::npos) {
        file = filename;
    } else {
        path = filename.mid(0, pos);
        file = filename.mid(pos + 1);
    }

    ::google::protobuf::compiler::DiskSourceTree sourceTree;
    sourceTree.MapPath("", path.toStdString());
    ::google::protobuf::compiler::Importer importer(&sourceTree, NULL);
    importer.Import(file.toStdString());
    const ::google::protobuf::Descriptor *descriptor
        = importer.pool()->FindMessageTypeByName(classname.toStdString());
    if(!descriptor) {
        return 1;
    }
    ::google::protobuf::DynamicMessageFactory factory;
    const ::google::protobuf::Message *message
        = factory.GetPrototype(descriptor);
    if(!message) {
        return 2;
    }
    ::google::protobuf::Message* msg = message->New();
    if(!msg) {
        return 3;
    }
    cb(msg);
    delete msg;
    return 0;
}
                                          
