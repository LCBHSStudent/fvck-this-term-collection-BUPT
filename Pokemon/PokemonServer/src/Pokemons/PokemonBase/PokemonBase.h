#pragma once

#include <PreCompile.h>

#define ARGS_LIST \
    std::string_view    _name,  \
    uint32              _id,    \
    uint32              _level, \
    uint32              _exp,   \
    uint32              _ATK,   \
    uint32              _HP,    \
    uint32              _DEF,   \
    uint32              _SPD,   \
    uint32              _type,  \
    uint32              _attr   \

class PokemonBase {
    friend class PokemonSkill;
public:
		 PokemonBase(ARGS_LIST);
	virtual
		~PokemonBase() = default;

public RESOURCE:
	enum class PokemonType: uint32 {
		HIGH_ATTACK = 0,
		HIGH_HITPOINT,
		HIGH_DEFENSE,
		HIGH_SPEED,
		UNDEFINED
	};
    enum class PokemonAttribute: uint32 {
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
    GET(uint32, SPD)
    GET(uint32, curSPD)
    
    GET(PokemonType, pkmType)
    GET(PokemonAttribute, pkmAttr)
    
    GET(std::string_view, name)
    
public FUNCTION:
	virtual uint32
		attack(
            PokemonBase&    target,
		    QString&        skillName
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
		m_SPD = 0, m_curSPD = 0;
    
	PokemonType
		m_pkmType  = PokemonType::UNDEFINED;
    PokemonAttribute
        m_pkmAttr  = PokemonAttribute::UNDEFINED;
    std::array<QString, 4>
        m_skills   = {};
};
