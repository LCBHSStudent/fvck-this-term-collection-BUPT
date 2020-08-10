#ifndef USER_H
#define USER_H

#include <PreCompile.h>

class PokemonBase;

class User: public QObject {
    Q_OBJECT
public:
        User(
			QString		username,
			QTcpSocket* socket
        );
		User(const User&);
    virtual 
        ~User();
public RESOURCE:
	enum class BadgeType: int {
		BRONZE = 0,
		SILVER,
		GOLDEN
	};
	enum class UserStatus: int {
		IDLE = 0,
		BATTLING,
        NOT_ONLINE,
		UNDEFINED,
	};
	
public FUNCTION:
	PROPERTY(double,        winRate)
    PROPERTY(int,           battlePkmId)
    PROPERTY(QString,       name)
	PROPERTY(BadgeType,     countBadge)
	PROPERTY(BadgeType,     qualityBadge)
	PROPERTY(UserStatus,	status)
	PROPERTY(QList<int>,    pokemonList)
	
	GET(QTcpSocket*,		userSocket)
	
	PokemonBase 
        requestUserPkmInfo(uint32 pkmId);
    void
        battleWon();
    void
        battleLose();
    void 
        updateUserInfo();
    
private FUNCTION:
    
private RESOURCE:
    int
        m_battlePkmId;
	QString
		m_name;
	double
		m_winRate;
	BadgeType
		m_countBadge;
	BadgeType
		m_qualityBadge;
	UserStatus
		m_status;
	QList<int>
		m_pokemonList;
	QTcpSocket*
		m_userSocket;
};

#endif // USER_H
