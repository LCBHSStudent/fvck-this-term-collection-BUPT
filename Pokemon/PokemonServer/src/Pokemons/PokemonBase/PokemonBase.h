#pragma once

#include <PreCompile.h>

class PokemonBase {
public:
		 PokemonBase() = default;
	virtual
		~PokemonBase() = default;


public RESOURCE:
	enum class PokemonType {
		HIGH_ATTACK = 0,
		HIGH_HITPOINT,
		HIGH_DEFENSE,
		HIGH_SPEED,
		UNDEFINED
	};
	enum class PokemonRare {
		R = 0,
		SR,
		SSR,
		UNDEFINED
	};

#ifndef QT_DEPENDECE
	enum class SkillType {

	};
#endif
	static const uint32 
        MAX_LEVEL = 15;
    
public FUNCTION:
	void
		gainExperience(uint32 exp);
    
    GET(uint32, id)
    GET(uint32, exp)
    GET(uint32, ATK)
    GET(uint32, curATK)
    GET(uint32, HP)
    GET(uint32, curHP)
    GET(uint32, DEF)
    GET(uint32, curDEF)
    GET(uint32, INTV)
    GET(uint32, curINTV)
    
    GET(PokemonType, pkmType)
    GET(PokemonRare, pkmRare)
    
    GET(std::string_view, name)
    
protected FUNCTION:
	virtual uint32
		attack(
			const uint32        skillIndex,
            const PokemonBase*  target
		) = 0;
	virtual void
		levelUp() = 0;

protected RESOURCE:
	std::string
		m_name  = {};
    uint32
        m_id    = 0;
    uint32
		m_level = 0;
	uint32
		m_exp	= 0;

	uint32
		m_ATK  = 0, m_curATK  = 0;
	uint32
		m_HP   = 0, m_curHP   = 0;
	uint32
		m_DEF  = 0, m_curDEF  = 0;
	uint32
		m_INTV = 0, m_curINTV = 0;

	PokemonType
		m_pkmType = PokemonType::UNDEFINED;
	PokemonRare
		m_pkmRare = PokemonRare::UNDEFINED;
	/*std::array< ,4>*/
};
