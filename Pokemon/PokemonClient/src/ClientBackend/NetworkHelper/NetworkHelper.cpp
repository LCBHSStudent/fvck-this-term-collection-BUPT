#include "NetworkHelper.h"

NetworkHelper::NetworkHelper(
    const QString   hostAddr,
    QObject         *parent
):  QObject(parent),
    m_host(hostAddr),
    m_status(false),
    m_nextBlockSize(0),
    m_socket(new QTcpSocket),
    m_timeoutTimer(new QTimer),
    m_keepAliveTimer(new QTimer)
{
    m_timeoutTimer->setSingleShot(true);
    
    m_keepAliveTimer->setSingleShot(false);
    m_keepAliveTimer->setInterval(heartBeatInterval);
    m_keepAliveTimer->setTimerType(Qt::TimerType::CoarseTimer);
    
    m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    
    connect(m_timeoutTimer, &QTimer::timeout,
            this, &NetworkHelper::connectionTimeout);
    connect(m_keepAliveTimer, &QTimer::timeout,
            this, &NetworkHelper::checkAndReconnect);
    
    connect(m_socket, &QTcpSocket::disconnected,
            this, &NetworkHelper::closeConnection);
    
    connect(m_socket, &QTcpSocket::connected,
            this, &NetworkHelper::connected);
    connect(m_socket, &QTcpSocket::readyRead,
            this, &NetworkHelper::readyRead);
    
    m_keepAliveTimer->start();
}

void NetworkHelper::connect2host() {
    m_timeoutTimer->start(connectLmt);
    m_socket->connectToHost(m_host, port);
}

void NetworkHelper::connectionTimeout() {
    qDebug() << "connection timeout";
    if(m_socket->state() == QAbstractSocket::ConnectingState) {
        m_socket->abort();
        emit m_socket->error(QAbstractSocket::SocketTimeoutError);
    }
}

void NetworkHelper::checkAndReconnect() {
    qDebug() << "check socket status";
    qDebug() << m_socket->state();
    if(m_socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "try to reconnect host machine";
        connect2host();
    }
}

void NetworkHelper::connected() {
    m_status = true;
    m_timeoutTimer->stop();
    qDebug() << "successfully connect to server";
    emit statusChanged(true);
}

bool NetworkHelper::getStatus() const {return m_status;}

void NetworkHelper::readyRead() {
    while (m_socket->bytesAvailable() >= sizeof(uint32)) {
        static bool flag = true;
        static uint32 length = 0;
        static char lengthArr[sizeof(uint32)] = {0};
        if (flag) {
            m_socket->read(lengthArr, sizeof(uint32));
            length = *reinterpret_cast<uint32*>(lengthArr);
        }
        if (length <= 0) {
            qDebug () << "coco";
            m_socket->readAll();
            flag = true;
//            throw std::runtime_error("")
        } else {
            if (length <= m_socket->bytesAvailable()) {
                flag = true;
                QByteArray data = m_socket->read(length);
                emit sigServerMessage(data);
            } else {
                flag = false;
                return;
            }
        }
    }
}

void NetworkHelper::closeConnection() {
    qDebug() << "disconnect from server";
    m_timeoutTimer->stop();
    
    // 防止服务端重启后客户端收不到已连接的信号
//    disconnect(m_socket, &QTcpSocket::connected, 0, 0);
//    disconnect(m_socket, &QTcpSocket::readyRead, 0, 0);
    
    bool shouldEmit = false;
    switch (m_socket->state()) {
    case 0:
        m_socket->disconnectFromHost();
        shouldEmit = true;
        break;
    case 2:
        m_socket->abort();
        shouldEmit = true;
        break;
    default:
        m_socket->abort();
        break;
    }
    
    if(shouldEmit) {
        m_status = false;
        emit statusChanged(false);
    }
}

NetworkHelper::~NetworkHelper() {
    if(m_status) {
        m_socket->disconnectFromHost();
    }
    delete m_socket;
    delete m_timeoutTimer;
}

void NetworkHelper::sendToServer(const QString& msg) {
    
}

void NetworkHelper::sendToServer(QByteArray&& data) {
    m_socket->write(data, data.length());
//    m_socket->flush();
//    m_socket->waitForBytesWritten();
}
