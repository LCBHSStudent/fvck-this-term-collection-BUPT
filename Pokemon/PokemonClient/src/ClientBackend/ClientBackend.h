#ifndef CLIENTBACKEND_H
#define CLIENTBACKEND_H

#include <PreCompile.h>

class NetworkHelper;

class ClientBackend : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(
        QString userName
        READ    getUserName
        NOTIFY  userNameChanged)
public:
    explicit
        ClientBackend(QString hostAddr);
    virtual
        ~ClientBackend();
        
signals:
    // 信号组，用于backend通知前端进行响应
    void    /*触发用户名变化的信号*/
        userNameChanged();
    void    /*收到服务端注册响应*/
        sigUserSignUp(int status);
    void    /*收到服务端登录响应*/
        sigUserLogin(int status);
    void    /*收到请求用户信息响应*/
        sigGetUserInfo(int status, QVariantMap userInfo);
    void    /*收到请求在线用户列表响应*/
        sigGetOnlineUserList(QList<QString> nameList, QList<int> statusList);
    void    /*收到各种模式下的精灵信息列表*/
        sigGetPokemonDataList(int mode, QString userName, QList<QVariantMap> pkmList);
    void    /*收到其他用户的对战请求*/
        sigGetBattleInviteRequest(QString fromUser, int battleMode);
    void    /*收到对战请求的应答*/
        sigGetBattleStartResponse(int status, int isUserA, int urPkmId, int taPkmId);
    void    /*收到对战回合信息*/
        sigGetBattleTurnInfo(QVariantMap info);
    void    /*收到对战结束信息*/
        sigGetBattleFinishInfo(int result, int mode);
    void    /*收到转移宝可梦*/
        sigTransferPokemonFinished(int result);
    
public slots:
    void    /*接收network helper组件送来的服务器数据报文*/
        slotGetServerMessage(QByteArray);

public FUNCTION:
    Q_INVOKABLE void    /*向服务器发送登录请求报文*/
        sendLoginRequest(QString username, QString password);
    Q_INVOKABLE void    /*向服务器发送注册请求报文*/
        sendSignUpRequest(QString username, QString password);
    Q_INVOKABLE void    /*向服务器发送获取在线用户列表请求报文*/
        sendOnlineUserListRequest();
    Q_INVOKABLE void    /*向服务器发送请求自身宝可梦信息的报文*/
        sendSelfPokemonInfoRequest();
    Q_INVOKABLE void    /*向服务器发送请求服务器宝可梦信息的报文*/
        sendServerPokemonInfoRequest();
    Q_INVOKABLE void    /*向服务器发送获取对战用宝可梦信息请求的报文*/
        sendBattlePokemonInfoRequest(QString taUserName, int myPkmId, int taPkmId);
    Q_INVOKABLE void    /*向服务器发送自身用户信息请求报文*/
        sendSelfUserInfoRequest();
    Q_INVOKABLE void    /*向服务器发送特顶用户信息请求的报文*/
        sendUserInfoRequest(QString username);
    Q_INVOKABLE void    /*向服务器发送请求对战开始的报文*/
        sendBattleStartRequest(int mode, int pkmId, QString destName, int serverPkm = 0);
    Q_INVOKABLE void    /*向服务器发送应答对战请求的报文*/
        sendBattleInviteResponse(int flag, int battleMode, int pkmId, QString fromUser);    
    Q_INVOKABLE void    /*向服务器发送回合内使用技能的报文*/
        sendBattleSkillIndex(int isUserA, int index);
    Q_INVOKABLE void    /*向服务器发送对战放弃报文*/
        sendBattleGiveupInfo();
    Q_INVOKABLE void    /*向服务器发送回转移宝可梦所有权的报文*/
        sendTransferPokemonRequest(QString fromUser, int pkmId);
    
    QString /*get接口 获取用户名称 基本供property bind使用*/
        getUserName() const {return m_userName;}
    
private RESOURCE:
    QString /*暂存服务端IP信息*/
        m_host = {};
    NetworkHelper*  /*网络组件（tcpsocket封装类指针)*/
        m_helper = nullptr;
    QString /*存储用户名的实际变量*/
        m_userName;// = QStringLiteral("admin");
};

#endif // CLIENTBACKEND_H
