#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include <PreCompile.h>

class NetworkHelper: public QObject {
    Q_OBJECT
public:
    explicit 
        NetworkHelper(
            const QString hostAddr,
            QObject *parent = nullptr
        );
    virtual 
        ~NetworkHelper();
    
    bool 
        getStatus() const;

public slots:
    void 
        closeConnection();
    void 
        connect2host();
    void
        sendToServer(const QString&);
    void
        sendToServer(QByteArray&&);

signals:
    void
        statusChanged(bool);
    void 
        hasReadSome(QString msg);

    
public RESOURCE:
    inline static int 
        port = 1919;
    const  static int 
        connectLmt = 3000;
    const  static int 
        heartBeatInterval = 15000;
    enum DataType {
        UserSignUpResponseInfo = 0,
        UserLoginResponseInfo,
        UserInfo,
    };
    
public FUNCTION:
    
private slots:
    void 
        readyRead();
    void 
        connected();
    void 
        connectionTimeout();
    void
        checkAndReconnect();
    
private RESOURCE:
    QString
        m_host;
    bool
        m_status;
    quint16 
        m_nextBlockSize;
    QTcpSocket*
        m_socket;
    QTimer*
        m_timeoutTimer;
    QTimer*
        m_keepAliveTimer;
};

#endif // NETWORKHELPER_H
