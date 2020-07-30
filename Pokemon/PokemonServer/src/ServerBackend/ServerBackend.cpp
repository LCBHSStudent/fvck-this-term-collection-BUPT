#include "ServerBackend.h"
#include <UserProtocol.pb.h>
#include <MessageTypeGlobal.h>
#include "../StorageHelper/StorageHelper.h"

#undef  NET_SLOT
#define NET_SLOT(_name) \
    void ServerBackend::slot##_name(QTcpSocket* client, QByteArray data)

#define CONNECT_EVENT(_eventName)                           \
    connect(                                                \
        m_helper, &NetworkHelper::sig##_eventName,    \
        this,     &ServerBackend::slot##_eventName)  \

#define CALL_SLOT(_name) \
    slot##_name(client, QByteArray(data.data()+4, data.size()-4))

// ----------------Progress Before Sending Network Data---------------- //
#define PROC_PROTODATA(_messageType, _dataBlockName) \
    auto byteLength = _dataBlockName.ByteSizeLong();                    \
    auto pData      = new char[_dataBlockName.ByteSizeLong() + 4];      \
    *reinterpret_cast<int*>(pData) = MessageType::_messageType;         \
                                                                        \
    _dataBlockName.SerializeToArray(pData + 4, byteLength);             \
                                                                        \
    m_helper->sendToClient(client, QByteArray(pData, byteLength + 4));  \
    delete[] pData                                                      \
// -------------------------------------------------------------------- //

ServerBackend::ServerBackend():
    m_helper(new NetworkHelper)
{
    StorageHelper::Instance();
    
    createUserTable("_server");
    // 添加服务器精灵
    // m_serverPkm.append()
    
    CONNECT_EVENT(GetMessage);
}

ServerBackend::~ServerBackend() {
    // RELEASE TCP SOCKET SERVER HELPER
    if(m_helper) {
        delete m_helper;
    }
    for(auto& pData: m_serverPkm) {
        delete pData;
        pData = nullptr;
    }
}

// 创建用户表，记录其拥有的宝可梦信息
void ServerBackend::createUserTable(const QString& username) {
    const QString userTableStat = 
"CREATE TABLE IF NOT EXISTS `user_" + username + "`(\
    PKM_ID      INT             NOT NULL PRIMARY KEY AUTO_INCREMENT,\
    PKM_TYPEID  INT             NOT NULL,\
    PKM_NAME    VARCHAR(128)    NOT NULL,\
    PKM_LEVEL   INT             NOT NULL DEFAULT 1,\
    PKM_EXP     INT             NOT NULL DEFAULT 0,\
    PKM_ATK     INT             NOT NULL DEFAULT 0,\
    PKM_DEF     INT             NOT NULL DEFAULT 0,\
    PKM_HP      INT             NOT NULL DEFAULT 0,\
    PKM_SPD     INT             NOT NULL DEFAULT 0\
);";
    StorageHelper::Instance().transaction(userTableStat, StorageHelper::DEFAULT_FUNC);
}

void ServerBackend::slotGetMessage(
    QTcpSocket*     client,
    QByteArray      data
) {
    const int type = *reinterpret_cast<int*>(data.data());
    
    switch (type) {
    case MessageType::UserSignUpRequest:
        CALL_SLOT(UserSignUp);
        break;
    case MessageType::UserLoginRequest:
        CALL_SLOT(UserLogin);
        break;
    default:
        qDebug() << "known message type";
        break;
    }
}

// -----------------------NETWORK TRANSACTION-------------------------- //
/*@ 参数类型 QTcpSocket* client, const QByteArray data
 *@ 用于处理接收到对应类型的网络信号后，通过protobuf序列化获取信息
 *@ 最后利用PROC_PROTODATA返回响应信息
 */

// 处理用户登录请求
NET_SLOT(UserLogin) {
    UserProtocol::UserLoginRequestInfo reqInfo = {};
    reqInfo.ParseFromArray(data.data(), data.size());
    
    qDebug() << "[ServerBackend] :UserLogin";
    reqInfo.PrintDebugString();
    
    bool    flag = false;
    QString userName = QString::fromStdString(reqInfo.username());
    QString userPsw;
    
    StorageHelper::Instance().transaction(
        "SELECT PASSWORD FROM `user_list` WHERE USERNAME=?",
        [&userPsw, &flag](QSqlQuery& query) {
            flag = true;
            userPsw = query.value(0).toString();
        },
        QString::fromStdString(reqInfo.username())
    );
    
    UserProtocol::UserLoginResponseInfo resInfo = {};
    if(flag) {
        if(userPsw == QString::fromStdString(reqInfo.userpsw())) {
            bool flag = true;
            resInfo.set_status(
                UserProtocol::UserLoginResponseInfo_LoginStatus_SUCCESS);
            for(int i = 0; i < m_userList.size(); i++) {
                if (userName == m_userList.at(i).get_name()) {
                    resInfo.set_status(
                        UserProtocol::UserLoginResponseInfo_LoginStatus_SERVER_REFUSED);
                    flag = false;
                    break;
                }
            }
            // TODO: 将 User 加入 UserList？
            if(flag) {
                
            }
            
        } else {
            resInfo.set_status(
                UserProtocol::UserLoginResponseInfo_LoginStatus_USERPSW_ERROR);
        }
    } else {
        resInfo.set_status(
            UserProtocol::UserLoginResponseInfo_LoginStatus_USER_NOT_EXISTS);
    }
    
    PROC_PROTODATA(UserLoginResponse, resInfo);
}

// 处理用户注册请求
NET_SLOT(UserSignUp) {
    UserProtocol::UserSignUpRequestInfo reqInfo = {};
    reqInfo.ParseFromArray(data.data(), data.size());
    
    qDebug() << "[ServerBackend]: UserSignUp"; 
    reqInfo.PrintDebugString();
    
    int count = 0;
    StorageHelper::Instance().transaction(
        "SELECT count(*) FROM `user_list` WHERE USERNAME=?",
        [&count](QSqlQuery& query){
            count = query.value(0).toInt();
        },
        QString::fromStdString(reqInfo.username())
    );
    
    UserProtocol::UserSignUpResponseInfo resInfo = {};
    if(count > 0) {
        qDebug() << "user already exist";
        resInfo.set_status(
            UserProtocol::UserSignUpResponseInfo_SignUpStatus_USER_ALREADY_EXISTS);
    } else {
        StorageHelper::Instance().transaction(
            "INSERT INTO user_list("
                "USERNAME, PASSWORD"
            ") VALUES(?, ?)", 
            StorageHelper::DEFAULT_FUNC,
            QString::fromStdString(reqInfo.username()),
            QString::fromStdString(reqInfo.userpsw())
        );
        createUserTable(QString::fromStdString(reqInfo.username()));
        resInfo.set_status(
            UserProtocol::UserSignUpResponseInfo_SignUpStatus_SUCCESS);
    }
    
    PROC_PROTODATA(UserSignUpResponse, resInfo);
}

// 处理获取用户信息
NET_SLOT(RequestUserInfo) {
    UserProtocol::UserInfoRequest reqInfo = {};
    reqInfo.ParseFromArray(data.data(), data.size());
    
    // 获取用户宝可梦信息
    std::vector<int> pkmIdList;
    int pkmCount = 0,
        highLevelPkmCnt = 0;
    StorageHelper::Instance().transaction(
        "SELECT PKM_ID, PKM_LEVEL FROM `user_" + 
        QString::fromStdString(reqInfo.username()) + "`",
        [&pkmIdList, &pkmCount, &highLevelPkmCnt](QSqlQuery& query) {
            pkmIdList.push_back(query.value(0).toInt());
            int level = query.value(1).toInt();
            if(level == 15) {
                highLevelPkmCnt++;
            }
            pkmCount++;
        }
    );
    
    int totalBattleTime = 0,
        totalWinnerTime = 0;
    // 获取用户信息(胜场胜率)
    StorageHelper::Instance().transaction(
        "SELECT TOTAL_BATTLE_TIME, WINNER_TIME FROM `user_list",
        [&totalBattleTime, &totalWinnerTime](QSqlQuery& query) {
            totalBattleTime = query.value(0).toInt();
            totalWinnerTime = query.value(1).toInt();
        }
    );
    
    UserProtocol::UserInfoResponse resInfo = {};
    if(pkmCount >= 20) {
        resInfo.set_pkmamountbadge(
            UserProtocol::UserInfoResponse_BadgeType_GOLD);
    } else if(pkmCount >= 10) {
        resInfo.set_pkmamountbadge(
            UserProtocol::UserInfoResponse_BadgeType_SILVER);
    } else {
        resInfo.set_pkmamountbadge(
            UserProtocol::UserInfoResponse_BadgeType_BRONZE);
    }
    
    if(highLevelPkmCnt >= 10) {
        resInfo.set_highlevelbadge(
            UserProtocol::UserInfoResponse_BadgeType_GOLD);
    } else if(highLevelPkmCnt >= 5) {
        resInfo.set_highlevelbadge(
            UserProtocol::UserInfoResponse_BadgeType_SILVER);
    } else {
        resInfo.set_highlevelbadge(
            UserProtocol::UserInfoResponse_BadgeType_BRONZE);
    }
    
    for(auto pkmId: pkmIdList) {
        resInfo.add_pokemonid(pkmId);
    }
    
    resInfo.set_timeofduel(totalBattleTime);
    resInfo.set_timeofwins(totalWinnerTime);
    
    PROC_PROTODATA(UserInfoResponse, resInfo);
}

// 处理用户登出 & 断线
NET_SLOT(UserLogout) {
    (void)client;
    (void)data;
}

// 处理获取宝可梦信息请求
NET_SLOT(RequestPkmInfo) {
    (void)client;
    
    UserProtocol::UserPokemonDataRequestInfo reqInfo = {};
    reqInfo.ParseFromArray(data.data(), data.size());
}


