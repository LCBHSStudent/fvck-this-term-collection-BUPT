#include "ClientBackend.h"

#include <UserProtocol.pb.h>
#include <MessageTypeGlobal.h>
#include "NetworkHelper/NetworkHelper.h"

// ----------------Progress Before Sending Network Data---------------- //
#define PROC_PROTODATA(_messageType, _dataBlockName) \
    auto byteLength = _dataBlockName.ByteSizeLong();                    \
    auto pData      = new char[_dataBlockName.ByteSizeLong() + 4];      \
    *reinterpret_cast<int*>(pData) = MessageType::_messageType;         \
                                                                        \
    _dataBlockName.SerializeToArray(pData + 4, byteLength);             \
                                                                        \
    m_helper->sendToServer(QByteArray(pData, byteLength + 4));          \
    delete[] pData                                                      \
// -------------------------------------------------------------------- //

ClientBackend::ClientBackend(QString hostAddr):
    QQuickItem(),
    m_host(hostAddr),
    m_helper(new NetworkHelper(hostAddr))
{
    connect(
        m_helper, &NetworkHelper::sigServerMessage,
        this, &ClientBackend::slotGetServerMessage
    );
    m_helper->connect2host();
}

ClientBackend::~ClientBackend() {
    if(m_helper) {
        delete m_helper;
    }
}

void ClientBackend::slotGetServerMessage(QByteArray data) {
    auto messageType = *reinterpret_cast<int*>(data.data());
    switch (messageType) {
    case UserSignUpResponse: {
        UserProtocol::UserSignUpResponseInfo info = {};
        info.ParseFromArray(data.data()+4, data.size()-4);
        emit sigUserSignUp(info.status());
    } break;
    case UserLoginResponse: {
        UserProtocol::UserLoginResponseInfo info = {};
        info.ParseFromArray(data.data()+4, data.size()-4);
        emit sigUserLogin(info.status());
    } break;
        
    default:
        break;
    }
}

void ClientBackend::sendLoginRequest(
    QString     username,
    QString     password
) {
    if(!m_helper->getStatus()) {
        qDebug() << "client offline";
        return;
    }
    m_userName = username;
    emit userNameChanged();
    
    UserProtocol::UserLoginRequestInfo info = {};
    info.set_userpsw(password.toStdString());
    info.set_username(username.toStdString());
    
    info.PrintDebugString();
    
    PROC_PROTODATA(UserLoginRequest, info);
}

void ClientBackend::sendSignUpRequest(
    QString     username,
    QString     password
) {
    if(!m_helper->getStatus()) {
        qDebug() << "client offline";
        return;
    }
    m_userName = username;
    emit userNameChanged();
    
    UserProtocol::UserSignUpRequestInfo info = {};
    info.set_userpsw(password.toStdString());
    info.set_username(username.toStdString());
    
    info.PrintDebugString();
    
    PROC_PROTODATA(UserSignUpRequest, info);
}
