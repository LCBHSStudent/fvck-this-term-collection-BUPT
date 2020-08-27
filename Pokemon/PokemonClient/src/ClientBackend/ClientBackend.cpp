#include "ClientBackend.h"

#include <UserProtocol.pb.h>
#include <BattleProtocol.pb.h>
#include <MessageTypeGlobal.h>
#include "NetworkHelper/NetworkHelper.h"

// ------------------Progress Before Sending Network Data----------------- //
// -------------------组装头部（length & type） -->填充内容------------------ //
#define PROC_PROTODATA(_messageType, _dataBlockName) \
    auto byteLength = _dataBlockName.ByteSizeLong();                        \
    auto pData      = new char[byteLength + 2*sizeof(uint32)];              \
    *(reinterpret_cast<uint32*>(pData+sizeof(uint32))) =                    \
                                            MessageType::_messageType;      \
    *reinterpret_cast<uint32*>(pData)       = (uint32)byteLength + 4;       \
                                                                            \
    _dataBlockName.SerializeToArray(pData + 2*sizeof(uint32), byteLength);  \
                                                                            \
    m_helper->sendToServer(QByteArray(pData, byteLength+2*sizeof(uint32))); \
    delete[] pData                                                          \
// ----------------------------------------------------------------------- //

// 用于检测socket连接状态的宏，不是非成员函数！
#define CHECK_SOCKET_STATUS \
    if(!m_helper->getStatus()) {        \
        qDebug() << "client offline";   \
        return;                         \
    }                                   \

/**
 * @brief ClientBackend::ClientBackend
 *        构造后台类
 * @param {QString}hostAddr 服务端IPV4-addr字符串
 */
ClientBackend::ClientBackend(QString hostAddr):
    QQuickItem(),
    m_host(hostAddr),
    m_helper(new NetworkHelper(hostAddr))
{
    connect(
        m_helper, &NetworkHelper::sigServerMessage,
        this, &ClientBackend::slotGetServerMessage
    );
    m_helper->connect2host();
}

/**
 * @brief ClientBackend::~ClientBackend
 *        后台类析构函数
 */
ClientBackend::~ClientBackend() {
    if(m_helper) {
        delete m_helper;
    }
}


/**
 * @brief ClientBackend::slotGetServerMessage
 *        接收到server信息时调用
 * @param {QByteArray} data 由网络辅助类送来的封装数据
 */
void ClientBackend::slotGetServerMessage(QByteArray data) {
    // 拿到开头的messageType
    auto messageType = *reinterpret_cast<int*>(data.data());
    // 根据MessageType进行处理
    switch (messageType) {
    case UserSignUpResponse: {
        UserProtocol::UserSignUpResponseInfo info = {};
        info.ParseFromArray(data.data()+4, data.size()-4);
        emit sigUserSignUp(info.status());
    } break;
        
    case UserLoginResponse: {
        UserProtocol::UserLoginResponseInfo info = {};
        info.ParseFromArray(data.data()+4, data.size()-4);
        emit sigUserLogin(info.status());
    } break;
    
    case OnlineUserListResponse: {
        UserProtocol::OnlineUserListResponseInfo info = {};
        info.ParseFromArray(data.data()+4, data.size()-4);
        
        QList<QString>  nameList;
        QList<int>      statusList;
        
        for (int i = 0; i < info.userlist_size(); i++) {
            QString name = QString::fromStdString(info.userlist(i).username());
            nameList.push_back(name);
            statusList.push_back(info.userlist(i).userstatus());
        }
        
        emit sigGetOnlineUserList(nameList, statusList);
    } break;
        
    case PokemonDataResponse: {
        UserProtocol::UserPokemonDataResponseInfo info = {};
        info.ParseFromArray(data.data()+4, data.size()-4);
        
        QList<QVariantMap> pkmList = {};
        
        int count = info.pkmdata_size();
        for (int i = 0; i < count; i++) {
            QVariantMap pkmData;
            const auto& data = info.pkmdata(i);
            
            pkmData["id"]       = data.id();
            pkmData["typeId"]   = data.typeid_();
            pkmData["name"]     = QString::fromStdString(data.name());
            pkmData["level"]    = data.level();
            pkmData["type"]     = data.type();
            pkmData["exp"]      = data.exp();
            pkmData["attr"]     = data.attr();
            pkmData["atk"]      = data.atk();
            pkmData["def"]      = data.def();
            pkmData["hp"]       = data.hp();
            pkmData["spd"]      = data.spd();
            pkmData["skill_1"]  = QString::fromStdString(data.skill_1());
            pkmData["skill_2"]  = QString::fromStdString(data.skill_2());
            pkmData["skill_3"]  = QString::fromStdString(data.skill_3());
            pkmData["skill_4"]  = QString::fromStdString(data.skill_4());
            pkmData["desc"]     = QString::fromStdString(data.desc());
            pkmData["desc_s1"]  = QString::fromStdString(data.skill_1_desc());
            pkmData["desc_s2"]  = QString::fromStdString(data.skill_2_desc());
            pkmData["desc_s3"]  = QString::fromStdString(data.skill_3_desc());
            pkmData["desc_s4"]  = QString::fromStdString(data.skill_4_desc());
            
//            qDebug() << pkmData["skill_1"];
//            qDebug() << pkmData["skill_2"];
//            qDebug() << pkmData["skill_3"];
//            qDebug() << pkmData["skill_4"];
            
            pkmList.push_back(pkmData);
        }
        
        emit sigGetPokemonDataList(
            info.mode(), QString::fromStdString(info.username()), pkmList);
        
    } break;
    
    case UserInfoResponse: {
        UserProtocol::UserInfoResponse info = {};
        info.ParseFromArray(data.data()+4, data.size()-4);
        
        QVariantMap userInfo = {};
        userInfo["userName"]    = QString::fromStdString(info.username());
        userInfo["timeOfDuel"]  = info.timeofduel();
        userInfo["timeOfWins"]  = info.timeofwins();
        userInfo["winRate"]     = info.winrate();
        userInfo["amountBadge"] = info.pkmamountbadge();
        userInfo["levelBadge"]  = info.highlevelbadge();
        userInfo["status"]      = info.status();
        
        emit sigGetUserInfo(info.resstatus(), userInfo);
    } break;
    
    case BattleInviteRequest: {
        BattleProtocol:: BattleInviteRequest info = {};
        info.ParseFromArray(data.data()+4, data.size()-4);
        
        emit sigGetBattleInviteRequest(
            QString::fromStdString(info.fromuser()), info.battlemode());
    } break;
        
    case BattleStartResponse: {
        BattleProtocol::BattleStartResponse info = {};
        info.ParseFromArray(data.data()+4, data.size()-4);
        
        emit sigGetBattleStartResponse(
            info.status(), info.isusera(), info.urpkmid(), info.tapkmid());
    } break;
    
    case BattleTurnInfo: {
        BattleProtocol::BattleTurnInfo info = {};
        info.ParseFromArray(data.data()+4, data.size()-4);
        
        info.PrintDebugString();
        
        qDebug() << "aaaaaaaaaaaaaaaaaaaaaaaaa" << info.type();
        
        QVariantMap data = {};
        data["type"]            = info.type();
        data["skillName"]       = QString::fromStdString(info.skillname());
        data["selfDeltaHP"]     = info.selfdeltahp();
        data["destDeltaHP"]     = info.destdeltahp();
        data["selfBuffID"]      = info.selfbuffid();
        data["selfBuffLast"]    = info.selfbufflast();
        data["destBuffID"]      = info.destbuffid();
        data["destBuffLast"]    = info.destbufflast();
        
        emit sigGetBattleTurnInfo(data);
    } break;
    
    case BattleFinishInfo: {
        BattleProtocol::BattleFinishInfo info = {};
        info.ParseFromArray(data.data()+4, data.size()-4);
        
        info.PrintDebugString();
        
        emit sigGetBattleFinishInfo(info.result(), info.mode());
    } break;
    
    case TransferPokemonResponse: {
        UserProtocol::TransferPokemonResponse info = {};
        info.ParseFromArray(data.data()+4, data.size()-4);
        
        info.PrintDebugString();
        
        emit sigTransferPokemonFinished(info.status());
    } break;
        
    default:
        break;
    }
}

/**
 * @brief ClientBackend::sendLoginRequest
 *        发送登录请求
 * @param username 用户名
 * @param password 密码
 */
void ClientBackend::sendLoginRequest(
    QString     username,
    QString     password
) {
    CHECK_SOCKET_STATUS;
    
    m_userName = username;
    emit userNameChanged();
    
    UserProtocol::UserLoginRequestInfo info = {};
    info.set_userpsw(password.toStdString());
    info.set_username(username.toStdString());
    
    info.PrintDebugString();
    
    PROC_PROTODATA(UserLoginRequest, info);
}

/**
 * @brief ClientBackend::sendSignUpRequest
 *        发送注册请求
 * @param username 用户名
 * @param password 密码
 */
void ClientBackend::sendSignUpRequest(
    QString     username,
    QString     password
) {
    CHECK_SOCKET_STATUS;
    
    m_userName = username;
    emit userNameChanged();
    
    UserProtocol::UserSignUpRequestInfo info = {};
    info.set_userpsw(password.toStdString());
    info.set_username(username.toStdString());
    
    info.PrintDebugString();
    
    PROC_PROTODATA(UserSignUpRequest, info);
}

/**
 * @brief ClientBackend::sendOnlineUserListRequest
 *        发送在线用户列表获取请求
 */
void ClientBackend::sendOnlineUserListRequest() {
    CHECK_SOCKET_STATUS;
    
    UserProtocol::OnlineUserListRequestInfo info = {};
    info.set_username(m_userName.toStdString());
    info.PrintDebugString();
    
    PROC_PROTODATA(OnlineUserListRequest, info);
}

/**
 * @brief ClientBackend::sendSelfPokemonInfoRequest
 *        发送获取自身宝可梦信息请求
 */
void ClientBackend::sendSelfPokemonInfoRequest() {
    CHECK_SOCKET_STATUS;
    
    UserProtocol::UserPokemonDataRequestInfo info = {};
    info.set_reqtype(
        UserProtocol::UserPokemonDataRequestInfo_PokemonDataRequestType_ALL);
    info.set_username(m_userName.toStdString());
    info.set_mode(UserProtocol::PokemonDataRequestMode::MAIN_PAGE);
    info.PrintDebugString();
    
    PROC_PROTODATA(PokemonDataRequest, info);
}

/**
 * @brief ClientBackend::sendSelfUserInfoRequest
 *        获取自身用户信息请求
 */
void ClientBackend::sendSelfUserInfoRequest() {
    sendUserInfoRequest(m_userName);
}

/**
 * @brief ClientBackend::sendUserAllPkmDataRequest
 *        向服务器请求特定用户的所有宝可梦信息
 * @param username {QString} 用户名
 */
void ClientBackend::sendUserAllPkmDataRequest(QString username) {
    CHECK_SOCKET_STATUS;
    
    UserProtocol::UserPokemonDataRequestInfo info = {};
    info.set_reqtype(
        UserProtocol::UserPokemonDataRequestInfo_PokemonDataRequestType_ALL);
    info.set_username(username.toStdString());
    info.set_mode(UserProtocol::PokemonDataRequestMode::USER_INFO_PAGE);
    info.PrintDebugString();
    
    PROC_PROTODATA(PokemonDataRequest, info);
}


/**
 * @brief ClientBackend::sendUserInfoRequest
 *        获取指定用户信息请求
 * @param username 用户名
 */
void ClientBackend::sendUserInfoRequest(QString username) {
    CHECK_SOCKET_STATUS;
    
    UserProtocol::UserInfoRequest info = {};
    info.set_username(username.toStdString());
    PROC_PROTODATA(UserInfoRequest, info);
}

/**
 * @brief ClientBackend::sendBattleStartRequest
 *        发送对战开始请求
 * @param mode      对战模式
 * @param pkmId     自身选择的宝可梦id
 * @param destName  对手用户名
 * @param serverPkm 如果是与server对战，则为指定的server宝可梦id
 */
void ClientBackend::sendBattleStartRequest(
    int     mode,
    int     pkmId,
    QString destName,
    int     serverPkm
) {
    CHECK_SOCKET_STATUS;
    
    BattleProtocol::BattleStartRequest info = {};
    if (mode == 0) {
        info.set_battlemode(BattleProtocol::BattleMode::EXP_BATTLE);
    } else {
        info.set_battlemode(BattleProtocol::BattleMode::DUEL_BATTLE);
    }
    info.set_fromuser(m_userName.toStdString());
    info.set_destuser(destName.toStdString());
    info.set_fromuserpkmid(pkmId);
    info.set_serverpkmid(serverPkm);
    
    PROC_PROTODATA(BattleInviteRequest, info);
}

/**
 * @brief ClientBackend::sendBattleInviteResponse
 *        发送对战邀请的回应
 * @param flag          接受 & 拒绝 &其他错误处理码
 * @param battleMode    对战模式
 * @param pkmId         自身选择的出战宝可梦id
 * @param fromUser      是来自哪个用户的邀请
 */
void ClientBackend::sendBattleInviteResponse(
    int     flag,
    int     battleMode,
    int     pkmId,
    QString fromUser
) {
    CHECK_SOCKET_STATUS;
    
    BattleProtocol::BattleInviteResponse resInfo = {};
    if (flag == 0) {
        resInfo.set_flag(BattleProtocol::BattleStartStatus::SUCCESS);
    } else if (flag == 1) {
        resInfo.set_flag(BattleProtocol::BattleStartStatus::REFUSED);
    }
    
    resInfo.set_destuser(m_userName.toStdString());
    resInfo.set_fromuser(fromUser.toStdString());
    resInfo.set_destuserpkmid(pkmId);
    
    if (battleMode == 0) {
        resInfo.set_battlemode(BattleProtocol::BattleMode::EXP_BATTLE);
    } else {
        resInfo.set_battlemode(BattleProtocol::BattleMode::DUEL_BATTLE);
    }
    
    PROC_PROTODATA(BattleInviteResponse, resInfo);
}

/**
 * @brief ClientBackend::sendBattleSkillIndex
 *        发送指定技能槽的index
 * @param isUserA   是UserA吗
 * @param index     skill槽的下标  
 */
void ClientBackend::sendBattleSkillIndex(int isUserA, int index) {
    CHECK_SOCKET_STATUS;

    BattleProtocol::BattleOperationInfo info = {};
    info.set_isusera(isUserA);
    info.set_skillindex(index);
    info.set_username(m_userName.toStdString());
    
    PROC_PROTODATA(BattleOperationInfo, info);
}

/**
 * @brief ClientBackend::sendBattleGiveupInfo
 *        发送对战放弃请求
 */
void ClientBackend::sendBattleGiveupInfo() {
    CHECK_SOCKET_STATUS;
    
    BattleProtocol::BattleGiveupInfo info = {};
    info.set_username(m_userName.toStdString());
    
    PROC_PROTODATA(BattleGiveupInfo, info);
}

/**
 * @brief ClientBackend::sendBattlePokemonInfoRequest
 *        获取对战用宝可梦信息
 * @param taUserName    对方用户名
 * @param myPkmId       我的宝可梦id
 * @param taPkmId       对方宝可梦id
 */
void ClientBackend::sendBattlePokemonInfoRequest(
    QString taUserName,
    int     myPkmId,
    int     taPkmId
) {
    CHECK_SOCKET_STATUS;
    
    UserProtocol::UserPokemonDataRequestInfo info1 = {};
    UserProtocol::UserPokemonDataRequestInfo info2 = {};
    
    info1.set_username(m_userName.toStdString());
    if (taUserName.length()) {
        info2.set_username(taUserName.toStdString());
    }
    else {
        info2.set_username("_server");
    }
    info1.set_mode(UserProtocol::PokemonDataRequestMode::BATTLE_START);
    info2.set_mode(UserProtocol::PokemonDataRequestMode::BATTLE_START);
    info1.set_reqtype(
        UserProtocol::UserPokemonDataRequestInfo_PokemonDataRequestType_SPECIFIC);
    info2.set_reqtype(
        UserProtocol::UserPokemonDataRequestInfo_PokemonDataRequestType_SPECIFIC);
    info1.add_pokemonid(myPkmId);
    info2.add_pokemonid(taPkmId);
    
    {
        info1.PrintDebugString();
        PROC_PROTODATA(PokemonDataRequest, info1);
    }
    {
        info2.PrintDebugString();
        PROC_PROTODATA(PokemonDataRequest, info2);
    }
}

/**
 * @brief ClientBackend::sendServerPokemonInfoRequest
 *        请求获取服务器宝可梦信息
 */
void ClientBackend::sendServerPokemonInfoRequest() {
    CHECK_SOCKET_STATUS;
    
    UserProtocol::UserPokemonDataRequestInfo info = {};
    info.set_reqtype(
        UserProtocol::UserPokemonDataRequestInfo_PokemonDataRequestType_ALL);
    info.set_username("_server");
    info.set_mode(UserProtocol::PokemonDataRequestMode::SERVER_PKM_LIST);
    info.PrintDebugString();
    
    PROC_PROTODATA(PokemonDataRequest, info);
}

/**
 * @brief ClientBackend::sendTransferPokemonRequest
 *        发送宝可梦所有权转移请求
 * @param fromUser  从谁那里
 * @param pkmId     转移哪一个
 */
void ClientBackend::sendTransferPokemonRequest(QString fromUser, int pkmId) {
    CHECK_SOCKET_STATUS;
    
    UserProtocol::TransferPokemonRequest info = {};
    info.set_pkmid(pkmId);
    info.set_destuser(fromUser.toStdString());
    info.set_fromuser(m_userName.toStdString());
    
    PROC_PROTODATA(TransferPokemonRequest, info);
}

//    QList<QString> nameList = {};
//    nameList.append("ASD");
//    nameList.append("ASD");
//    nameList.append("ASD");
//    nameList.append("ASDASDASDASDASDASD");
//    nameList.append("奇酷比");
    
//    QList<int> statusList;
//    statusList.append(1);
//    statusList.append(1);
//    statusList.append(1);
//    statusList.append(0);
//    statusList.append(0);
    
//    emit sigGetOnlineUserList(nameList, statusList);
