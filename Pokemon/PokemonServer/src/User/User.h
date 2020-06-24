#ifndef USER_H
#define USER_H

#include <PreCompile.h>

class User: public QObject {
    Q_OBJECT
public:
	User();
    
public RESOURCE:
	enum class BadgeType: uint32 {
		GOLDEN = 0,
		SILVER,
		BRONZE
	};
	
public FUNCTION:
	GET(double, winRate)
    GET(std::string, name)
	GET(BadgeType, countBadge)
	GET(BadgeType, qualityBadge)
	GET(QList<uint32>, pokemonList)
    
private RESOURCE:
	std::string
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
