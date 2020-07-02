#include "ServerBackend.h"
#include "../StorageHelper/StorageHelper.h"

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
    
    createUserTable("_server");
    // m_serverPkm.append()
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

void ServerBackend::createUserTable(const QString& username) {
    QSqlQuery createQuery(m_db);
    const QString userTableStat = 
"CREATE TABLE IF NOT EXISTS `user_" + username + "`(\
    PKM_ID      INT             NOT NULL PRIMARY KEY,\
    PKM_NAME    VARCHAR(128)    NOT NULL,\
    PKM_LEVEL   INT             NOT NULL DEFAULT 1,\
    PKM_TYPE    INT             NOT NULL DEFAULT 0,\
    PKM_EXP     INT             NOT NULL DEFAULT 0,\
    PKM_ATTR    INT             NOT NULL DEFAULT 0,\
    PKM_ATK     INT             NOT NULL DEFAULT 0,\
    PKM_DEF     INT             NOT NULL DEFAULT 0,\
    PKM_HP      INT             NOT NULL DEFAULT 0,\
    PKM_SPD     INT             NOT NULL DEFAULT 0,\
    PKM_SKILL_1 VARCHAR(64)     NOT NULL,\
    PKM_SKILL_2 VARCHAR(64)     NOT NULL,\
    PKM_SKILL_3 VARCHAR(64)     NOT NULL,\
    PKM_SKILL_4 VARCHAR(64)     NOT NULL\
);";
    StorageHelper::Instance().transaction(userTableStat);
}
