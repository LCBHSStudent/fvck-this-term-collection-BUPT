#include "ServerBackend.h"
#include <UserProtocol.pb.h>
#include "../StorageHelper/StorageHelper.h"

#undef NET_SLOT
#define NET_SLOT(_name) \
    void ServerBackend::slot##_name(const QByteArray data)
#define CONNECT_EVENT(_eventName) \
    connect(                                        \
        m_helper.get(), &NetworkHelper::sig##_eventName,  \
        this,            &ServerBackend::slot##_eventName  \
    )\

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
    // CONNECT SLOTS AND SIGNALS
    {
        CONNECT_EVENT(UserLogin);
        CONNECT_EVENT(UserSignUp);
        CONNECT_EVENT(RequestUserInfo);
        CONNECT_EVENT(UserLogout);
        CONNECT_EVENT(RequestPkmInfo);
    }
    
    // 添加服务器精灵
    // m_serverPkm.append()
}

ServerBackend::~ServerBackend() {
    // RELEASE TCP SOCKET SERVER HELPER
    m_helper.release();
    for(auto& pData: m_serverPkm) {
        delete pData;
        pData = nullptr;
    }
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

NET_SLOT(UserLogin) {
    UserProtocol::UserLoginRequestInfo info = {};
}

NET_SLOT(UserSignUp) {
    UserProtocol::UserSignUpRequestInfo info = {};
    info.ParseFromArray(data.data(), data.size());
    
    info.PrintDebugString();
}

NET_SLOT(RequestUserInfo) {
    
}

NET_SLOT(UserLogout) {
    
}

NET_SLOT(RequestPkmInfo) {
    
}
