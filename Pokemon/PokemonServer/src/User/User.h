#ifndef USER_H
#define USER_H

#include <PreCompile.h>

class PokemonBase;

class User: public QObject {
    Q_OBJECT
public:
        User(QString username, QObject* parent = nullptr);
    virtual 
        ~User();
public RESOURCE:
	enum class BadgeType: uint32 {
		GOLDEN = 0,
		SILVER,
		BRONZE
	};
	
public FUNCTION:
	GET(double,         winRate)
    GET(QString,        name)
	GET(BadgeType,      countBadge)
	GET(BadgeType,      qualityBadge)
	GET(QList<uint32>,  pokemonList)
	
	PokemonBase 
        requestUserPkmInfo(uint32 pkmId);
    void
        battleWon();
    void
        battleFailed();
    
private FUNCTION:
    void 
        updateUserInfo();
    
private RESOURCE:
	QString
		m_name;
	double
		m_winRate;
	BadgeType
		m_countBadge;
	BadgeType
		m_qualityBadge;
	QList<uint32>
		m_pokemonList;
};

#endif // USER_H
