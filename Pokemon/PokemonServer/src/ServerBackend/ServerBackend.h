#ifndef SERVERBACKEND_H
#define SERVERBACKEND_H

#include <PreCompile.h>
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
    
public FUNCTION:
    
    
private RESOURCE:
    std::unique_ptr<NetworkHelper>
        m_helper;
    QSqlDatabase
        m_db {};
    
};

#endif // SERVERBACKEND_H
