#include "StorageHelper.h"

QMutex StorageHelper::
    m_instMtx  = QMutex();
StorageHelper* StorageHelper::
    m_instance = nullptr;

StorageHelper::StorageHelper() {
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setDatabaseName ("pokemon");
    m_db.setUserName("root");
    m_db.setPassword ("password");
    
    if(!m_db.open()) {
        throw std::runtime_error("failed to connect local mysql");
    }
    
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
}
