#ifndef CLIENTBACKEND_H
#define CLIENTBACKEND_H

#include <PreCompile.h>

class NetworkHelper;

class ClientBackend : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(
        QString userName
        READ    getUserName
        NOTIFY  userNameChanged)
public:
    explicit
        ClientBackend(QString hostAddr);
    virtual
        ~ClientBackend();
        
signals:
    void 
        userNameChanged();
    void
        sigUserSignUp(int status);
    void
        sigUserLogin(int status);
    void
        sigGetOnlineUserList(QList<QString> nameList, QList<uint32> statusList);
    
public slots:
    void
        slotGetServerMessage(QByteArray);

public FUNCTION:
    Q_INVOKABLE void
        sendLoginRequest(QString username, QString password);
    Q_INVOKABLE void
        sendSignUpRequest(QString username, QString password);
    Q_INVOKABLE void
        sendOnlineUserListRequest();
    
    QString
        getUserName() const {return m_userName;}
    
private RESOURCE:
    QString 
        m_host = {};
    NetworkHelper*
        m_helper = nullptr;
    QString
        m_userName = QStringLiteral("测试");
};

#endif // CLIENTBACKEND_H
