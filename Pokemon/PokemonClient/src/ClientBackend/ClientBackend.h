#ifndef CLIENTBACKEND_H
#define CLIENTBACKEND_H

#include <PreCompile.h>

class NetworkHelper;

class ClientBackend : public QQuickItem {
    Q_OBJECT
public:
    explicit
        ClientBackend(QString hostAddr);
    virtual
        ~ClientBackend();
        
signals:
        
public FUNCTION:
    Q_INVOKABLE void
        sendLoginRequest(QString username, QString password);
    Q_INVOKABLE void
        sendSignUpRequest(QString username, QString password);
    
private RESOURCE:
    QString 
        m_host = {};
    NetworkHelper*
        m_helper = nullptr;
};

#endif // CLIENTBACKEND_H
