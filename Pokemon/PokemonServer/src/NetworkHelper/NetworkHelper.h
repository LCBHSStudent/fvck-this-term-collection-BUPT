#ifndef SOCKETHELPER_H
#define SOCKETHELPER_H

#include <PreCompile.h>

class NetworkHelper: public QObject {
    Q_OBJECT
public:
    explicit NetworkHelper(QObject *parent = nullptr);
    
public FUNCTION:
    size_t
        sendToClient(QTcpSocket*, const QString&);    
    
public RESOURCE:
    inline static int port = 1919;
    enum DataType {
        UserSignUpRequestInfo = Qt::UserRole,
        UserLoginRequestInfo,
    };
    
private slots:
    void
        slotNewConnection();
    void 
        slotReadClient();
    void
        slotGotDisconnection();
    
private RESOURCE:
    QList<QTcpSocket*>
        m_clients {};
    QTcpServer*
        m_server = nullptr;
};

#endif // SOCKETHELPER_H
