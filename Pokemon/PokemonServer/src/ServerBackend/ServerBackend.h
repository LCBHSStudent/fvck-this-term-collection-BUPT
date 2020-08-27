#ifndef SERVERBACKEND_H
#define SERVERBACKEND_H

#include <PreCompile.h>
#include <UserProtocol.pb.h>
#include <BattleProtocol.pb.h>
#include "../User/User.h"
#include "../BattleField/BattleField.h"
#include "../NetworkHelper/NetworkHelper.h"
#include "../Pokemons/PokemonBase/PokemonFactory.h"

#define NET_SLOT(_name) \
    void slot##_name(QTcpSocket* client, QByteArray data)

class ServerBackend: public QObject {
    Q_OBJECT
public:
    ServerBackend();
    ~ServerBackend();
     
public slots:
    // 读取到某个客户端消息并处理
    NET_SLOT(GetMessage);
    
private FUNCTION:
    void    // 创建用户表
        createUserTable(const QString& username);
    void    // 转移宝可梦所有权
        transferPokemon(
            const QString&  fromUser,
            const QString&  destUser,
            int             pkmId
        );
    void    // 删除用户宝可梦
        removeUserPokemon(
            const QString&  userName,
            int             pkmId
        );
    void    // 发送List内的宝可梦信息到目的socket
        procAndSendPkmData(
            const QList<int>&                      pkmIdList,
            const QString&                         userName,
            QTcpSocket*                            destSocket,
            UserProtocol::PokemonDataRequestMode   mode
        );

    
private slots:
    // 网络消息槽组   参数皆为 {QTcpSocket*} client 与 {QByteArray} data
    NET_SLOT(UserLogin);
    NET_SLOT(UserSignUp);
    NET_SLOT(RequestPkmInfo);
    NET_SLOT(RequestUserInfo);
    NET_SLOT(UserDisconnected);
    NET_SLOT(RequestOnlineUserList);
    NET_SLOT(BattleInvite);
    NET_SLOT(HandleBattleInviteResponse);
    NET_SLOT(HandleBattleOperation);
    NET_SLOT(TransferPokemon);
    
    void    /*获取到BattleField内的回合信息*/
        slotGetTurnInfo(BattleField::TurnInfo info);
    void    /*获取到battlefield内的对战结果信息*/
        slotGetBattleResult(User* winner);
    
private RESOURCE:
    NetworkHelper*          /*网络辅助类*/
        m_helper;
    QHash<QString, User>    /*用户名到User*的Hash（暂未启用）*/ 
        m_userHash;
    QList<User>             /*用户列表*/
        m_userList = {};
    QList<BattleField*>     /*对战组件列表*/
        m_battleFieldList = {};
};

#endif // SERVERBACKEND_H
