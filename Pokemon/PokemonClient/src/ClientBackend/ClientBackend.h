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
    void 
        userNameChanged();
    void
        sigUserSignUp(int status);
    void
        sigUserLogin(int status);
    void
        sigGetUserInfo(int status, QVariantMap userInfo);
    void
        sigGetOnlineUserList(QList<QString> nameList, QList<int> statusList);
    void
        sigGetPokemonDataList(int mode, QString userName, QList<QVariantMap> pkmList);
    void
        sigGetBattleInviteRequest(QString fromUser, int battleMode);
    void
        sigGetBattleStartResponse(int status, int isUserA, int urPkmId, int taPkmId);
    void
        sigGetBattleTurnInfo(QVariantMap info);
    void
        sigGetBattleFinishInfo(int result, int mode);
    
public slots:
    void
        slotGetServerMessage(QByteArray);

public FUNCTION:
    Q_INVOKABLE void
        sendLoginRequest(QString username, QString password);
    Q_INVOKABLE void
        sendSignUpRequest(QString username, QString password);
    Q_INVOKABLE void
        sendOnlineUserListRequest();
    Q_INVOKABLE void
        sendSelfPokemonInfoRequest();
    Q_INVOKABLE void
        sendServerPokemonInfoRequest();
    Q_INVOKABLE void
        sendBattlePokemonInfoRequest(QString taUserName, int myPkmId, int taPkmId);
    Q_INVOKABLE void
        sendSelfUserInfoRequest();
    Q_INVOKABLE void
        sendUserInfoRequest(QString username);
    Q_INVOKABLE void
        sendBattleStartRequest(int mode, int pkmId, QString destName, int serverPkm = 0);
    Q_INVOKABLE void
        sendBattleInviteResponse(int flag, int battleMode, int pkmId, QString fromUser);    
    Q_INVOKABLE void
        sendBattleSkillIndex(int isUserA, int index);
    Q_INVOKABLE void
        sendBattleGiveupInfo();
    
    QString
        getUserName() const {return m_userName;}
    
private RESOURCE:
    QString 
        m_host = {};
    NetworkHelper*
        m_helper = nullptr;
    QString
        m_userName = QStringLiteral("admin");
};

#endif // CLIENTBACKEND_H
