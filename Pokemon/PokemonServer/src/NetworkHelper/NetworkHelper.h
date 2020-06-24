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
        UserSignUpRequestInfo = 0,
        UserLoginRequestInfo,
    };
	
signals:
	void 
		sigUserLogin(QString& user, QString& password);
	void
		sigUserLogout(QString& user);
	void
		sigRequestUserInfo(QString& user);
	void 
		sigRequestPokemonInfo(QString& user);
	void
		sigStartUserBattle(QString& reqUser, QString& destUser);
	void
		sigStartVirtualBattle(QString& reqUser);
	
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
