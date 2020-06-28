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
    
    QSqlQuery createTable(m_db);
    const QString userTableStat = "CREATE TABLE IF NOT EXISTS pokemon_skills(\
    SKILL           VARCHAR(128)    NOT NULL    PRIMARY KEY,\
    DESCRIPTION     VARCHAR(256)    NOT NULL\
);";
    createTable.prepare(userTableStat);
    createTable.exec();
    qDebug() << (
        createTable.lastError().text().isEmpty()?
        "Create skills table succeed":
        createTable.lastError().text()
    );
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
