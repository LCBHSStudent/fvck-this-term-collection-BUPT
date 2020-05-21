#pragma once

#include <PreCompile.h>

class PokemonBase {
    friend class PokemonSkill;
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
    enum class PokemonAttribute {
        FIRE = 0,
        WATER,
        GRASS,
        UNDEFINED
    };

	static const uint32 
        MAX_LEVEL = 15;
    static int
        LEVEL_UP_EXP[15];
    
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
    GET(PokemonAttribute, pkmAttri)
    
    GET(std::string_view, name)
    
protected FUNCTION:
	virtual uint32
		attack(
            PokemonBase&    target,
		    uint32          skillIndex
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
    PokemonAttribute
        m_pkmAttri = PokemonAttribute::UNDEFINED;
};
