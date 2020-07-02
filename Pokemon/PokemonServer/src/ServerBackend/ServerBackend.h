#ifndef SERVERBACKEND_H
#define SERVERBACKEND_H

#include <PreCompile.h>
#include "../User/User.h"
#include "../BattleField/BattleField.h"
#include "../NetworkHelper/NetworkHelper.h"

class ServerBackend: public QObject {
    Q_OBJECT
public:
    ServerBackend();
    
public slots:
    void
        slotUserLogin();
    void
        slotUserLogout();
    void
        slotUserSingUp();
    void
        slotRequestUserInfo();
    void 
        slotRequestPokemonInfo();
    
private FUNCTION:
    void
        createUserTable(const QString& username);
    
private RESOURCE:
    std::unique_ptr<NetworkHelper>
        m_helper;
    QSqlDatabase
        m_db = {};
    QList<User>
        m_userList = {};
    
    QList<PokemonBase*>
        m_serverPkm = {};
};

#endif // SERVERBACKEND_H
