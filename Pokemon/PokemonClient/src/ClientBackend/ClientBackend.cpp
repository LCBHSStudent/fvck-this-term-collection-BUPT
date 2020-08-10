#include "ClientBackend.h"

#include <UserProtocol.pb.h>
#include <BattleProtocol.pb.h>
#include <MessageTypeGlobal.h>
#include "NetworkHelper/NetworkHelper.h"

// ------------------Progress Before Sending Network Data----------------- //
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

#define CHECK_SOCKET_STATUS \
    if(!m_helper->getStatus()) {        \
        qDebug() << "client offline";   \
        return;                         \
    }                                   \


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

ClientBackend::~ClientBackend() {
    if(m_helper) {
        delete m_helper;
    }
}

void ClientBackend::slotGetServerMessage(QByteArray data) {
    auto messageType = *reinterpret_cast<int*>(data.data());
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

void ClientBackend::sendOnlineUserListRequest() {
    CHECK_SOCKET_STATUS;
    
    UserProtocol::OnlineUserListRequestInfo info = {};
    info.set_username(m_userName.toStdString());
    info.PrintDebugString();
    
    PROC_PROTODATA(OnlineUserListRequest, info);
}

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

void ClientBackend::sendSelfUserInfoRequest() {
    sendUserInfoRequest(m_userName);
}

void ClientBackend::sendUserInfoRequest(QString username) {
    CHECK_SOCKET_STATUS;
    
    UserProtocol::UserInfoRequest info = {};
    info.set_username(username.toStdString());
    PROC_PROTODATA(UserInfoRequest, info);
}

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

void ClientBackend::sendBattleSkillIndex(int isUserA, int index) {
    CHECK_SOCKET_STATUS;

    BattleProtocol::BattleOperationInfo info = {};
    info.set_isusera(isUserA);
    info.set_skillindex(index);
    info.set_username(m_userName.toStdString());
    
    PROC_PROTODATA(BattleOperationInfo, info);
}

void ClientBackend::sendBattleGiveupInfo() {
    CHECK_SOCKET_STATUS;
    
    BattleProtocol::BattleGiveupInfo info = {};
    info.set_username(m_userName.toStdString());
    
    PROC_PROTODATA(BattleGiveupInfo, info);
}

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

void ClientBackend::sendTransferPokemonRequest(QString fromUser, int pkmId) {
    CHECK_SOCKET_STATUS;
    
    UserProtocol::TransferPokemonRequest info = {};
    info.set_pkmid(pkmId);
    info.set_destuser(m_userName.toStdString());
    info.set_fromuser(fromUser.toStdString());
    
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
