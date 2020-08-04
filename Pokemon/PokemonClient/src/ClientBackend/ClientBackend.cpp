#include "ClientBackend.h"

#include <UserProtocol.pb.h>
#include <BattleProtocol.pb.h>
#include <MessageTypeGlobal.h>
#include "NetworkHelper/NetworkHelper.h"

// ----------------Progress Before Sending Network Data---------------- //
#define PROC_PROTODATA(_messageType, _dataBlockName) \
    auto byteLength = _dataBlockName.ByteSizeLong();                    \
    auto pData      = new char[_dataBlockName.ByteSizeLong() + 4];      \
    *reinterpret_cast<int*>(pData) = MessageType::_messageType;         \
                                                                        \
    _dataBlockName.SerializeToArray(pData + 4, byteLength);             \
                                                                        \
    m_helper->sendToServer(QByteArray(pData, byteLength + 4));          \
    delete[] pData                                                      \
// -------------------------------------------------------------------- //

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
            
            pkmList.push_back(pkmData);
        }
        
        emit sigGetPokemonDataList(0, pkmList);
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

void ClientBackend::sendBattleStartRequest(int mode, QString destName) {
    CHECK_SOCKET_STATUS;
    
    BattleProtocol::BattleStartRequest info = {};
    info.set_battlemode(mode);
    info.set_fromuser(m_userName.toStdString());
    info.set_destuser(destName.toStdString());
    
    PROC_PROTODATA(BattleInviteRequest, info);
}

void ClientBackend::sendBattleInviteResponse(
    int     flag,
    int     battleMode,
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
    resInfo.set_battlemode(battleMode);
    
    PROC_PROTODATA(BattleInviteResponse, resInfo);
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
