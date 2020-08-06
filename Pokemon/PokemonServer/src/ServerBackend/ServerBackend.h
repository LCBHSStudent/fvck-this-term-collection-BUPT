#ifndef SERVERBACKEND_H
#define SERVERBACKEND_H

#include <PreCompile.h>
#include "../User/User.h"
#include "../BattleField/BattleField.h"
#include "../NetworkHelper/NetworkHelper.h"
#include "../Pokemons/PokemonBase/PokemonFactory.h"

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
    NET_SLOT(RequestPkmInfo);
    NET_SLOT(RequestUserInfo);
    NET_SLOT(UserDisconnected);
    NET_SLOT(RequestOnlineUserList);
    NET_SLOT(BattleInvite);
    NET_SLOT(HandleBattleInviteResponse);
    NET_SLOT(HandleBattleOperation);
    
    void
        slotGetTurnInfo(BattleField::TurnInfo info);
    void
        slotGetBattleResult(User* winner);
    
private RESOURCE:
    NetworkHelper* 
        m_helper;
    QHash<QString, User> 
        m_userHash;
    QList<User>
        m_userList = {};
    QList<BattleField*>
        m_battleFieldList = {};
};

#endif // SERVERBACKEND_H
