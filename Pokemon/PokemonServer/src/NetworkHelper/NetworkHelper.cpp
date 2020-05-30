#include "NetworkHelper.h"

NetworkHelper::NetworkHelper(QObject *parent):
    QObject(parent),
    m_server(new QTcpServer)
{
    if(!m_server->listen(QHostAddress::Any, NetworkHelper::port)) {
        throw std::runtime_error("failed to listen local port");
    } else {
        connect(
            m_server, &QTcpServer::newConnection,
            this, &NetworkHelper::slotNewConnection
        );
    }
}

void NetworkHelper::slotNewConnection() {
    QTcpSocket* clntSocket = m_server->nextPendingConnection();
    
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

void NetworkHelper::slotReadClient() {
    QTcpSocket* clntSocket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray data = clntSocket->readAll();
    
    qDebug() << data.length();
}

void NetworkHelper::slotGotDisconnection() {
    m_clients.removeAt(
        m_clients.indexOf(reinterpret_cast<QTcpSocket*>(sender()))
    );
    
    // emit
}

size_t NetworkHelper::sendToClient(
    QTcpSocket*     socket,
    const QString&  msg
) {
    return 0;
}
