#ifndef SERVERBACKEND_H
#define SERVERBACKEND_H

#include <PreCompile.h>
#include "../User/User.h"
#include "../BattleField/BattleField.h"
#include "../NetworkHelper/NetworkHelper.h"

#define NET_SLOT(_name) \
    void slot##_name(QTcpSocket* client, QByteArray data)

class ServerBackend: public QObject {
    Q_OBJECT
public:
    ServerBackend();
    ~ServerBackend();
     
public slots:
    NET_SLOT(GetMessage);
    
private FUNCTION:
    void
        createUserTable(const QString& username);
    
private slots:
    NET_SLOT(UserLogin);
    NET_SLOT(UserSignUp);
    NET_SLOT(UserLogout);
    NET_SLOT(RequestPkmInfo);
    NET_SLOT(RequestUserInfo);
    
private RESOURCE:
    NetworkHelper* 
        m_helper;
    QList<User>
        m_userList = {};
    
    QList<PokemonBase*>
        m_serverPkm = {};
};

#endif // SERVERBACKEND_H
