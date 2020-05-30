#include "ClientBackend.h"

#include <UserProtocol.pb.h>
#include "NetworkHelper/NetworkHelper.h"

ClientBackend::ClientBackend(QString hostAddr):
    QQuickItem(),
    m_host(hostAddr),
    m_helper(new NetworkHelper(hostAddr))
{
    m_helper->connect2host();
}

ClientBackend::~ClientBackend() {
    if(m_helper) {
        delete m_helper;
    }
}

void ClientBackend::sendLoginRequest(
    QString username,
    QString password
) {
    
}
void ClientBackend::sendSignUpRequest(
    QString username,
    QString password
) {
    if(!m_helper->getStatus()) {
        qDebug() << "client offline";
        return;
    }
    UserProtocol::UserSignUpRequestInfo info = {};
    info.set_userpsw(password.toStdString());
    info.set_username(username.toStdString());
    
    qDebug() << info.ByteSizeLong();
    
    auto byteLength = info.ByteSizeLong();
    auto data       = new char[info.ByteSizeLong()];
    
    info.SerializeToArray(data, byteLength);
    
    m_helper->sendToServer(QByteArray(data, byteLength));
    // m_helper->sendToServer();
}
