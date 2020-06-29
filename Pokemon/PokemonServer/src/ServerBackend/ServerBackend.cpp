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
    
    createTable();
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

void ServerBackend::createTable() const {
    QSqlQuery createQuery(m_db);
//---------创建技能说明索引表------------//   
    const QString skillTableStat = 
"CREATE TABLE IF NOT EXISTS skill_list(\
    NAME            VARCHAR(64)     NOT NULL    PRIMARY KEY,\
    ALIAS           VARCHAR(64)     NOT NULL,\
    DESCRIPTION     VARCHAR(256)    NOT NULL\
);";
    createQuery.prepare(skillTableStat);
    createQuery.exec();
    qDebug() << (
        createQuery.lastError().text().isEmpty()?
        "Create skill list table succeed":
        createQuery.lastError().text()
    );
    
    createQuery.clear();
    
    const QString userTableStat = 
"CREATE TABLE IF NOT EXISTS user_list(\
    USERNAME            VARCHAR(128)    NOT NULL    PRIMARY KEY,\
    PASSWORD            VARCHAR(128)    NOT NULL,\
    TOTAL_BATTLE_TIME   INT             NOT NULL    DEFAULT 0,\
    WINNER_TIME         INT             NOT NULL    DEFAULT 0\
);";
    createQuery.prepare(userTableStat);
    createQuery.exec();
    qDebug() << (
        createQuery.lastError().text().isEmpty()?
        "Create user list table succeed":
        createQuery.lastError().text()
    );
    
    createQuery.clear();
}

void ServerBackend::createUserTable(const QString& username) const {
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
    
    createQuery.prepare(userTableStat);
    createQuery.exec();
    qDebug() << (
        createQuery.lastError().text().isEmpty()?
        "Create user_" + username + " table succeed":
        createQuery.lastError().text()
    );
    
    createQuery.clear();     
}
