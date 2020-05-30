#include "ServerBackend.h"

ServerBackend::ServerBackend():
    m_helper(new NetworkHelper)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setDatabaseName ("pokemon");
    m_db.setUserName("root");
    m_db.setPassword ("password");
    if(!m_db.open()) {
        throw std::runtime_error("failed to connect local mysql");
    }
    
}

void ServerBackend::slotUserLogin() {
    
}

void ServerBackend::slotUserLogout() {
    
}

void ServerBackend::slotUserSingUp() {
    
}

void ServerBackend::slotRequestUserInfo() {
    
}

void ServerBackend::slotRequestPokemonInfo() {
    
}
