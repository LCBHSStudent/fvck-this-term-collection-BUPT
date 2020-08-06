#include "ServerBackend.h"
#include <UserProtocol.pb.h>
#include <BattleProtocol.pb.h>
#include <MessageTypeGlobal.h>
#include "../StorageHelper/StorageHelper.h"

#undef  NET_SLOT
#define NET_SLOT(_name) \
    void ServerBackend::slot##_name(QTcpSocket* client, QByteArray data)

#define CONNECT_EVENT(_eventName)                    \
    connect(                                         \
        m_helper, &NetworkHelper::sig##_eventName,   \
        this,     &ServerBackend::slot##_eventName)  \

#define CALL_SLOT(_name) \
    slot##_name(client, QByteArray(data.data()+4, data.size()-4))

// ----------------Progress Before Sending Network Data---------------- //
#define PROC_PROTODATA(_messageType, _dataBlockName) \
    auto byteLength = _dataBlockName.ByteSizeLong();                    \
    auto pData      = new char[byteLength + 4];                         \
    *reinterpret_cast<int*>(pData) = MessageType::_messageType;         \
                                                                        \
    _dataBlockName.SerializeToArray(pData + 4, byteLength);             \
                                                                        \
    m_helper->sendToClient(client, QByteArray(pData, byteLength + 4));  \
    delete[] pData                                                      \
// -------------------------------------------------------------------- //
#define PROC_PROTODATA_WITH_DEST(_messageType, _dataBlockName, _client) \
    auto byteLength = _dataBlockName.ByteSizeLong();                    \
    auto pData      = new char[byteLength + 4];                         \
    *reinterpret_cast<int*>(pData) = MessageType::_messageType;         \
                                                                        \
    _dataBlockName.SerializeToArray(pData + 4, byteLength);             \
                                                                        \
    m_helper->sendToClient(_client, QByteArray(pData, byteLength + 4)); \
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
    // 更换为direct connection?
    CONNECT_EVENT(UserDisconnected);
}

ServerBackend::~ServerBackend() {
    // RELEASE ALL BATTLE FIELD
    for (auto battle: m_battleFieldList) {
        delete battle;
    }
    // RELEASE TCP SOCKET SERVER HELPER    
    if(m_helper) {
        delete m_helper;
    }
}

// 创建用户表，记录其拥有的宝可梦信息
void ServerBackend::createUserTable(const QString& username) {
    const QString userTableStat = 
"CREATE TABLE IF NOT EXISTS `user_" + username + "`(\
    PKM_ID      INT             NOT NULL PRIMARY KEY AUTO_INCREMENT,\
    PKM_TYPEID  INT             NOT NULL,\
    PKM_LEVEL   INT             NOT NULL DEFAULT 1,\
    PKM_EXP     INT             NOT NULL DEFAULT 0,\
    PKM_ATK     INT             NOT NULL DEFAULT 6,\
    PKM_DEF     INT             NOT NULL DEFAULT 6,\
    PKM_HP      INT             NOT NULL DEFAULT 12,\
    PKM_SPD     INT             NOT NULL DEFAULT 6\
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
    case MessageType::UserInfoRequest:
        CALL_SLOT(RequestUserInfo);
        break;
    case MessageType::OnlineUserListRequest:
        CALL_SLOT(RequestOnlineUserList);
        break;
    case MessageType::PokemonDataRequest:
        CALL_SLOT(RequestPkmInfo);
        break;
    case MessageType::BattleInviteRequest:
        CALL_SLOT(BattleInvite);
        break;        
    case MessageType::BattleInviteResponse:
        CALL_SLOT(HandleBattleInviteResponse);
        break;
    case MessageType::BattleOperationInfo:
        CALL_SLOT(HandleBattleOperation);
        break;
//    case MessageType::BattleStartRequest:
//        CALL_SLOT(StartBattle);
//        break;
        
    default:
        qDebug() << "unknown message type";
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
            resInfo.set_status(
                UserProtocol::UserLoginResponseInfo_LoginStatus_SUCCESS);
            for(int i = 0; i < m_userList.size(); i++) {
                if (userName == m_userList.at(i).get_name()) {
                    resInfo.set_status(
                        UserProtocol::UserLoginResponseInfo_LoginStatus_SERVER_REFUSED);
                    break;
                }
            }
            // TODO: 将 User 加入 UserList？ -- finished
            {
                User curUser(userName, client);
                m_userList.push_back(curUser);
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
    
    int     count = 0;
    QString userName = QString::fromStdString(reqInfo.username());
    
    StorageHelper::Instance().transaction(
        "SELECT count(*) FROM `user_list` WHERE USERNAME=?",
        [&count](QSqlQuery& query){
            count = query.value(0).toInt();
        },
        userName
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
            userName,
            QString::fromStdString(reqInfo.userpsw())
        );
        createUserTable(userName);
        
        QList<int> typeIdList = {};
        StorageHelper::Instance().transaction(
            "SELECT PKM_ID FROM `pokemon_info`", 
            [&typeIdList](QSqlQuery& query) {
                typeIdList.push_back(query.value(0).toInt());
            }
        );
        
        for (auto i = 0; i < 3; i++) {
            int randIndex = 
                QRandomGenerator::global()->bounded(typeIdList.size());
            
            StorageHelper::Instance().transaction(
                "INSERT INTO `user_" +  userName + "`(\
                 PKM_TYPEID) VALUES(?)",
                 StorageHelper::DEFAULT_FUNC,
                 typeIdList[randIndex]
            );
        }
        
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
    
    int totalBattleTime = -1,
        totalWinnerTime = -1;
    // 获取用户信息(胜场胜率)
    StorageHelper::Instance().transaction(
        "SELECT TOTAL_BATTLE_TIME, WINNER_TIME FROM `user_list`",
        [&totalBattleTime, &totalWinnerTime](QSqlQuery& query) {
            totalBattleTime = query.value(0).toInt();
            totalWinnerTime = query.value(1).toInt();
        }
    );
    
    UserProtocol::UserInfoResponse resInfo = {};
    
    if (totalBattleTime == -1 ||
        totalWinnerTime == -1 ||
        pkmIdList.size() == 0
    ) {
        resInfo.set_resstatus(
            UserProtocol::UserInfoResponse_UserInfoResponseStatus_USER_NOT_EXIST);
    } else {
        if (pkmCount >= 20) {
            resInfo.set_pkmamountbadge(
                UserProtocol::UserInfoResponse_BadgeType_GOLDEN);
        } else if(pkmCount >= 10) {
            resInfo.set_pkmamountbadge(
                UserProtocol::UserInfoResponse_BadgeType_SILVER);
        } else {
            resInfo.set_pkmamountbadge(
                UserProtocol::UserInfoResponse_BadgeType_BRONZE);
        }
        
        if (highLevelPkmCnt >= 10) {
            resInfo.set_highlevelbadge(
                UserProtocol::UserInfoResponse_BadgeType_GOLDEN);
        } else if (highLevelPkmCnt >= 5) {
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
        resInfo.set_username(reqInfo.username());
        resInfo.set_resstatus(
            UserProtocol::UserInfoResponse_UserInfoResponseStatus_SUCCESS);
    }
    
    
    PROC_PROTODATA(UserInfoResponse, resInfo);
}

// 处理用户登出 & 断线
NET_SLOT(UserDisconnected) {
    (void)data;
    
    for (int i = 0; i < m_userList.size(); i++) {
        if (m_userList[i].get_userSocket() == client) {
            // 查找状态，若为对战中则向对手发送胜利报文
            for (int i = 0; i < m_battleFieldList.size(); i++) {
                auto battle = m_battleFieldList.at(i);
                
                User* dest = nullptr;
                if (battle->getUserA()->get_name() == m_userList[i].get_name()) {
                    dest = battle->getUserB();
                } else if (battle->getUserB()->get_name() == m_userList[i].get_name()) {
                    dest = battle->getUserA();
                }
                
                if (dest != nullptr) {
                    dest->set_status(User::UserStatus::IDLE);
                    
                    BattleProtocol::BattleFinishInfo info = {};
                    info.set_mode(
                        BattleProtocol::BattleFinishInfo_FinishMode_OPPOSITE_DISCONNECTED
                    );
                    info.set_result(
                        BattleProtocol::BattleFinishInfo_BattleResult_WIN
                    );
                    
                    
                    PROC_PROTODATA_WITH_DEST(
                        BattleFinishInfo, info, dest->get_userSocket()
                    );
                    break;
                }
                
                delete battle;
                m_battleFieldList.removeAt(i);
            }
            
            m_userList.removeAt(i);
        }
    }
    
    qDebug () 
            << "[SERVER BACKEND] CURRENT BATTLE FIELD COUNT: " 
            << m_battleFieldList.size();
    qDebug () 
            << "[SERVER BACKEND] CURRENT ONLINE USER COUNT: " 
            << m_userList.size();
}

// 处理获取宝可梦信息请求
NET_SLOT(RequestPkmInfo) {
    UserProtocol::UserPokemonDataRequestInfo reqInfo = {};
    reqInfo.ParseFromArray(data.data(), data.size());
    
    QString userName = QString::fromStdString(reqInfo.username());
#ifdef DEBUG_FLAG
    for (auto& user: m_userList) {
        if (user.get_userSocket() == client) {
            qDebug() << "USER SOCKET FOUND";
        }
    }
#endif
    
    UserProtocol::UserPokemonDataResponseInfo resInfo   = {};
    UserProtocol::PokemonInfo *pPkmInfo                 = nullptr;
    
    // ------ 拿到ID
    QList<int> pkmIdList = {};
//    QVector<PokemonBase*> ptrList = {};

    if (reqInfo.reqtype() ==
        UserProtocol::UserPokemonDataRequestInfo_PokemonDataRequestType_ALL
    ) {
        StorageHelper::Instance().transaction(
            "SELECT PKM_ID FROM `user_" + userName + "`",
            [&pkmIdList](QSqlQuery& query) {
                pkmIdList.push_back(query.value(0).toInt());
            }
        );
//        ptrList.resize(pkmIdList.size());
        
    } else if (reqInfo.reqtype() == 
        UserProtocol::UserPokemonDataRequestInfo_PokemonDataRequestType_SPECIFIC
    ) {
        for (int i = 0; i < reqInfo.pokemonid_size(); i++) {
            pkmIdList.push_back(reqInfo.pokemonid(i));
        }
    }
    
    QList<QString> aliasList = {};
    QList<QString> sdescList = {};
    // ------ 建立对象，转发数据
    for (int i = 0; i < pkmIdList.size(); i++) {
        aliasList.clear();
        sdescList.clear();
        auto pkm = PokemonFactory::CreatePokemon(userName, pkmIdList[i]);
        // 写入info
        pPkmInfo = resInfo.add_pkmdata();
        
        pPkmInfo->set_id(pkm->get_id());
        pPkmInfo->set_hp(pkm->get_HP());
        pPkmInfo->set_exp(pkm->get_exp());
        pPkmInfo->set_spd(pkm->get_SPD());
        pPkmInfo->set_def(pkm->get_DEF());
        pPkmInfo->set_atk(pkm->get_ATK());
        pPkmInfo->set_level(pkm->get_level());
        pPkmInfo->set_attr(pkm->get_pkmAttr());
        pPkmInfo->set_type(pkm->get_pkmType());
        pPkmInfo->set_typeid_(pkm->get_typeID());
        
        pPkmInfo->set_name(pkm->get_name().toStdString());
        pPkmInfo->set_desc(pkm->get_desc().toStdString());
        
        // 发送中文技能名 & 技能描述
        for (int i = 0; i < 4; i++) {
            StorageHelper::Instance().transaction(
                "SELECT ALIAS, DESCRIPTION FROM `skill_list` WHERE NAME=?",
                [&aliasList, &sdescList](QSqlQuery& query) {
                    aliasList.push_back(query.value(0).toString());
                    sdescList.push_back(query.value(1).toString());
                },
                pkm->getSkill(i)
            );
        }
        for (int i = 0; i < 4; i++) {
            qDebug() << pkm->getSkill(i);
            qDebug() << aliasList[i] << sdescList[i];            
        }
        
        
        pPkmInfo->set_skill_1(aliasList[0].toStdString());
        pPkmInfo->set_skill_2(aliasList[1].toStdString());
        pPkmInfo->set_skill_3(aliasList[2].toStdString());
        pPkmInfo->set_skill_4(aliasList[3].toStdString());
        
        pPkmInfo->set_skill_1_desc(sdescList[0].toStdString());
        pPkmInfo->set_skill_2_desc(sdescList[1].toStdString());
        pPkmInfo->set_skill_3_desc(sdescList[2].toStdString());
        pPkmInfo->set_skill_4_desc(sdescList[3].toStdString());
        
        qDebug() << "BYTE SIZE: " << pPkmInfo->ByteSizeLong();
        // pPkmInfo->PrintDebugString();
        
        delete pkm;
    }
    
    // PRINT DEBUG INFO
    {
        qDebug() << "BYTE SIZE: " << resInfo.ByteSizeLong();
        // resInfo.PrintDebugString();        
    }
    
    PROC_PROTODATA(PokemonDataResponse, resInfo);
}

// 处理获取在线用户列表请求
NET_SLOT(RequestOnlineUserList) {
    UserProtocol::OnlineUserListRequestInfo reqInfo = {};
    reqInfo.ParseFromArray(data.data(), data.size());
    
    reqInfo.PrintDebugString();
    
    QString requestUser = QString::fromStdString(reqInfo.username());
    
    UserProtocol::OnlineUserListResponseInfo resInfo = {};
    for (auto& user: m_userList) {
        qDebug() << "ONLINE USER: " << user.get_name();
        if (user.get_name() != requestUser) {
            UserProtocol::UserStatusInfo info = {};
            info.set_username(user.get_name().toStdString());
            info.set_userstatus((int32_t)user.get_status());
            
            *resInfo.add_userlist() = info;
        }
    }
    
    PROC_PROTODATA(OnlineUserListResponse, resInfo);
}

// 处理收到客户端发送的对战请求信息
// 记得Debug时走一下客户端的登录流程
NET_SLOT(BattleInvite) {
    BattleProtocol::BattleStartRequest info = {};
    info.ParseFromArray(data.data(), data.size());
#ifdef DEBUG_FLAG
    qDebug() << "[SERVER BACKEDN]: GET NEW BATTLE INVITE REQUEST";
    info.PrintDebugString();
#endif
    QString fromUser = QString::fromStdString(info.fromuser());
    QString destUser = QString::fromStdString(info.destuser());
    
    User* userA = nullptr;
    User* userB = nullptr;
    
    for (auto& user: m_userList) {
        if (user.get_name() == fromUser) {
            userA = &user;
            break;
        }
    }
    if (userA == nullptr) {
        return;
    }
    if (userA->get_status() == User::UserStatus::BATTLING) {
        return;
    }
    
    if (destUser == "_server") {
        {
            auto pkmA = PokemonFactory::CreatePokemon(fromUser, 1);
            auto pkmB = PokemonFactory::CreatePokemon(destUser, 1);
            auto battle = new BattleField(userA, nullptr, pkmA, pkmB);
            connect(
                battle, &BattleField::sigTurnInfoReady,
                this,   &ServerBackend::slotGetTurnInfo
            );
            connect(
                battle, &BattleField::sigBattleFinished,
                this,   &ServerBackend::slotGetBattleResult
            );
            m_battleFieldList.push_back(battle);
        }
        
        userA->set_status(User::UserStatus::BATTLING);
        
        BattleProtocol::BattleStartResponse resInfo = {};
        resInfo.set_status(BattleProtocol::BattleStartStatus::SUCCESS);
        resInfo.set_isusera(1);
        resInfo.set_tapkmid(1);
        resInfo.set_urpkmid(1);
        resInfo.PrintDebugString();
        
        PROC_PROTODATA(BattleStartResponse, resInfo);
        return;
    }
    else {
        for (auto& user: m_userList) {
            if (user.get_name() == destUser) {
                userB = &user;
#ifdef DEBUG_FLAG
                qDebug() << "[BATTLE INVITE] DEST USER FOUND";
#endif
                break;
            }
        }
        
        BattleProtocol::BattleStartResponse resInfo = {};
        if (userB == nullptr) {
            resInfo.set_status(BattleProtocol::BattleStartStatus::DEST_NOT_ONLINE);
        } else {
            if (userB->get_status() == User::UserStatus::BATTLING) {
                resInfo.set_status(BattleProtocol::BattleStartStatus::ALREADY_START);
            } else {
                BattleProtocol::BattleInviteRequest inviteInfo = {};
                inviteInfo.set_fromuser(info.fromuser());
                inviteInfo.set_battlemode(info.battlemode());
                
                inviteInfo.PrintDebugString();
                PROC_PROTODATA_WITH_DEST(
                    BattleInviteRequest, inviteInfo, userB->get_userSocket());
                return;
            }
        }
        resInfo.PrintDebugString();
        PROC_PROTODATA(BattleStartResponse, resInfo);
    }
}

NET_SLOT(HandleBattleInviteResponse) {
    BattleProtocol::BattleInviteResponse resInfo = {};
    resInfo.ParseFromArray(data.data(), data.size());
    
#ifdef DEBUG_FLAG
    qDebug () << "[SERVER BACKEND] GET BATTLE INVITE RESPONSE";
    resInfo.PrintDebugString();
#endif
    
    
    BattleProtocol::BattleStartResponse startInfoA = {};
    BattleProtocol::BattleStartResponse startInfoB = {};
    startInfoA.set_status(resInfo.flag());
    startInfoB.set_status(resInfo.flag());
    
    QString userNameA   = QString::fromStdString(resInfo.fromuser());
    QString userNameB   = QString::fromStdString(resInfo.destuser());
    User*   pUserA  = nullptr;
    User*   pUserB  = nullptr;
    
    for (auto& user: m_userList) {
        if (user.get_name() == userNameA) {
            pUserA  = &user;
        } else if (user.get_name() == userNameB) {
            pUserB  = &user;
        }
    }
    
    if (pUserA == nullptr || pUserB == nullptr) {
        qDebug() << "[SERVER BACKEND] NULL USER PTR IN HANDLING BATTLE INVITE RESPONSE";
        return;
    }
    
    startInfoA.set_isusera(1);
    startInfoB.set_isusera(0);
    
    int pkmId_A = 1;
    int pkmId_B = 2;
    
    startInfoA.set_urpkmid(pkmId_A);
    startInfoB.set_tapkmid(pkmId_A);
    startInfoA.set_tapkmid(pkmId_B);
    startInfoB.set_urpkmid(pkmId_B);
    
    startInfoA.PrintDebugString();
    startInfoB.PrintDebugString();
    
    // avoid redefination of variables
    {
        PROC_PROTODATA_WITH_DEST(
            BattleStartResponse, startInfoA, pUserA->get_userSocket());
    }
    {
        PROC_PROTODATA_WITH_DEST(
            BattleStartResponse, startInfoA, pUserA->get_userSocket());
    }
}

NET_SLOT(HandleBattleOperation) {
    BattleProtocol::BattleOperationInfo info = {};
    info.ParseFromArray(data.data(), data.size());
    
    info.PrintDebugString();
    
    int     isUserA     = info.isusera();
    int     skillIndex  = info.skillindex();
    QString userName    = QString::fromStdString(info.username());
    
    for (int i = 0; i < m_battleFieldList.size(); i++) {
        auto battle = m_battleFieldList[i];
        if (isUserA) {
            if (battle->getUserA()->get_name() == userName) {
                battle->setAction(skillIndex, 0);
                if (battle->getUserB() == nullptr) {
                    battle->setAction(
                        QRandomGenerator::global()->bounded(4), 1);
                }
            }
        } else {
            if (battle->getUserB()->get_name() == userName) {
                battle->setAction(skillIndex, 1);
            }
        }
    }
}

void ServerBackend::slotGetTurnInfo(BattleField::TurnInfo info) {
    BattleProtocol::BattleTurnInfo infoA = {};
    BattleProtocol::BattleTurnInfo infoB = {};
    
    if (info.type == BattleField::A_TO_B) {
        infoA.set_type(BattleField::A_TO_B);
        infoA.set_skillname(info.skillName.toStdString());
        infoA.set_selfbuffid(info.selfBuff.buffId);
        infoA.set_selfbufflast(info.selfBuff.turnCnt);
        infoA.set_destbuffid(info.destBuff.buffId);
        infoA.set_destbufflast(info.destBuff.turnCnt);
        infoA.set_selfdeltahp(info.selfDeltaHP);
        infoA.set_destdeltahp(info.destDeltaHP);
        
        infoB.set_type(BattleField::A_TO_B);
        infoB.set_skillname(info.skillName.toStdString());
        infoB.set_selfbuffid(info.destBuff.buffId);
        infoB.set_selfbufflast(info.destBuff.turnCnt);
        infoB.set_destbuffid(info.selfBuff.buffId);
        infoB.set_destbufflast(info.selfBuff.turnCnt);
        infoB.set_selfdeltahp(info.destDeltaHP);
        infoB.set_destdeltahp(info.selfDeltaHP);
    } else {
        infoB.set_type(BattleField::B_TO_A);
        infoB.set_skillname(info.skillName.toStdString());
        infoB.set_selfbuffid(info.selfBuff.buffId);
        infoB.set_selfbufflast(info.selfBuff.turnCnt);
        infoB.set_destbuffid(info.destBuff.buffId);
        infoB.set_destbufflast(info.destBuff.turnCnt);
        infoB.set_selfdeltahp(info.selfDeltaHP);
        infoB.set_destdeltahp(info.destDeltaHP);
        
        infoA.set_type(BattleField::A_TO_B);
        infoA.set_skillname(info.skillName.toStdString());
        infoA.set_selfbuffid(info.destBuff.buffId);
        infoA.set_selfbufflast(info.destBuff.turnCnt);
        infoA.set_destbuffid(info.selfBuff.buffId);
        infoA.set_destbufflast(info.selfBuff.turnCnt);
        infoA.set_selfdeltahp(info.destDeltaHP);
        infoA.set_destdeltahp(info.selfDeltaHP);
    }
    infoA.PrintDebugString();
    infoB.PrintDebugString();
    
    auto pBattleField = reinterpret_cast<BattleField*>(sender());
    {
        PROC_PROTODATA_WITH_DEST(
            BattleTurnInfo, infoA, pBattleField->getUserA()->get_userSocket());
    }
    {
        PROC_PROTODATA_WITH_DEST(
            BattleTurnInfo, infoB, pBattleField->getUserB()->get_userSocket());
    }
}

void ServerBackend::slotGetBattleResult(User* winner) {
    BattleProtocol::BattleFinishInfo infoA = {};
    BattleProtocol::BattleFinishInfo infoB = {};
    
    auto pBattleField = reinterpret_cast<BattleField*>(sender());
    if (winner == pBattleField->getUserA()) {
        infoA.set_mode(BattleProtocol::BattleFinishInfo_FinishMode_NORMAL);
        infoA.set_result(BattleProtocol::BattleFinishInfo_BattleResult_WIN);
        
        infoB.set_mode(BattleProtocol::BattleFinishInfo_FinishMode_NORMAL);
        infoB.set_result(BattleProtocol::BattleFinishInfo_BattleResult_LOSE);
    } else {
        infoB.set_mode(BattleProtocol::BattleFinishInfo_FinishMode_NORMAL);
        infoB.set_result(BattleProtocol::BattleFinishInfo_BattleResult_WIN);
        
        infoA.set_mode(BattleProtocol::BattleFinishInfo_FinishMode_NORMAL);
        infoA.set_result(BattleProtocol::BattleFinishInfo_BattleResult_LOSE);
    }
    infoA.PrintDebugString();
    infoB.PrintDebugString();
    {
        PROC_PROTODATA_WITH_DEST(
            BattleFinishInfo, infoA, pBattleField->getUserA()->get_userSocket());
    }
    {
        PROC_PROTODATA_WITH_DEST(
            BattleFinishInfo, infoB, pBattleField->getUserB()->get_userSocket());
    }
}
