#ifndef USER_H
#define USER_H

#include <PreCompile.h>

class PokemonBase;

class User: public QObject {
    Q_OBJECT
public:
        User(                   /*User构造函数*/
			QString		username,
			QTcpSocket* socket
        );
		User(const User&);      /*User拷贝构造函数数*/
    virtual                     /*User析构函数*/
        ~User();                
public RESOURCE:
    /**
     * @brief The BadgeType enum
     */
	enum class BadgeType: int {
		BRONZE = 0,
		SILVER,
		GOLDEN
	};
    
    /**
     * @brief The UserStatus enum
     */
	enum class UserStatus: int {
		IDLE = 0,
		BATTLING,
        NOT_ONLINE,
		UNDEFINED,
	};
	
public FUNCTION:
    // ------ Property属性组
	PROPERTY(double,        winRate)
    PROPERTY(int,           battlePkmId)
    PROPERTY(QString,       name)
	PROPERTY(BadgeType,     countBadge)
	PROPERTY(BadgeType,     qualityBadge)
	PROPERTY(UserStatus,	status)
	PROPERTY(QList<int>,    pokemonList)
	
    // 获取用户socket
	GET(QTcpSocket*,		userSocket)
	
	PokemonBase     /*获取用户信息*/
        requestUserPkmInfo(uint32 pkmId);
    void            /*战斗胜利*/
        battleWon();
    void            /*战斗失败*/
        battleLose();
    void            /*更新用户信息*/
        updateUserInfo();
    
private FUNCTION:
    
private RESOURCE:
    int             /*出战宝可梦ID*/
        m_battlePkmId;
	QString         /*用户名*/
		m_name;
	double          /*胜率*/
		m_winRate;
	BadgeType       /*宝可梦数量勋章类型*/
		m_countBadge;
	BadgeType       /*宝可梦质量勋章类型*/
		m_qualityBadge;
	UserStatus      /*用户状态*/
		m_status;
	QList<int>      /*用户宝可梦列表*/
		m_pokemonList;
	QTcpSocket*     /*用户对应的tcp socket*/
		m_userSocket;
};

#endif // USER_H
