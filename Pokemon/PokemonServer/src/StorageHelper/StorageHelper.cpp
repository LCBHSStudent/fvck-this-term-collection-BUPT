#include "StorageHelper.h"

QMutex StorageHelper::
    m_instMtx  = QMutex();
StorageHelper* StorageHelper::
    m_instance = nullptr;

/**
 * @brief StorageHelper::StorageHelper
 *        存储辅助类构造
 */
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
#ifdef DEBUG_FLAG
    qDebug() << (
        createQuery.lastError().text().isEmpty()?
        "Create skill list table succeed":
        createQuery.lastError().text()
    );
#endif
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
#ifdef DEBUG_FLAG
    qDebug() << (
        createQuery.lastError().text().isEmpty()?
        "Create user list table succeed":
        createQuery.lastError().text()
    );
#endif
    createQuery.clear();
        
    const QString pkmTableStat = 
"CREATE TABLE IF NOT EXISTS pokemon_info(\
    PKM_ID      INT             NOT NULL PRIMARY KEY,\
    PKM_NAME    VARCHAR(128)    NOT NULL,\
    PKM_TYPE    INT             NOT NULL DEFAULT 0,\
    PKM_ATTR    INT             NOT NULL DEFAULT 0,\
    ATK_INC     INT             NOT NULL DEFAULT 0,\
    DEF_INC     INT             NOT NULL DEFAULT 0,\
     HP_INC     INT             NOT NULL DEFAULT 0,\
    SPD_INC     INT             NOT NULL DEFAULT 0,\
    PKM_SKILL_1 VARCHAR(64)     NOT NULL,\
    PKM_SKILL_2 VARCHAR(64)     NOT NULL,\
    PKM_SKILL_3 VARCHAR(64)     NOT NULL,\
    PKM_SKILL_4 VARCHAR(64)     NOT NULL,\
    DESCRIPTION VARCHAR(256)    NOT NULL\
);";
    createQuery.prepare(pkmTableStat);
    createQuery.exec();
    createQuery.clear();
    
    const QString updateConfigStat1 = "set  global  interactive_timeout=31536000";
    createQuery.prepare(updateConfigStat1);
    createQuery.exec();
    createQuery.clear();
    
    const QString updateConfigStat2 = "set  global  wait_timeout=31536000";
    createQuery.prepare(updateConfigStat2);
    createQuery.exec();
#ifdef DEBUG_FLAG
    qDebug() << (
        createQuery.lastError().text().isEmpty()?
        "Create skill list table succeed":
        createQuery.lastError().text()
    );
#endif
}
