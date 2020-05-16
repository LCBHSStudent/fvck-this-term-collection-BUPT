#pragma once

#include <PreCompile.h>

#define FUNCTION 
#define RESOURCE 

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
	static const uint32 MAX_LEVEL = 15;

public FUNCTION:
	void
		gainExperience(uint32 exp);
	std::string_view
		getName() { return m_name; }


protected FUNCTION:
	virtual uint32
		attack(
			const uint32 skillIndex
		) = 0;
	virtual void
		levelUp() = 0;

protected RESOURCE:
	std::string
		m_name {};
	uint32
		m_level = 0;
	uint32
		m_exp	= 0;

	uint32
		m_atk  = 0, m_curATK  = 0;
	uint32
		m_hp   = 0, m_curHP   = 0;
	uint32
		m_def  = 0, m_curDEF  = 0;
	uint32
		m_intv = 0, m_curINTV = 0;

	PokemonType
		m_pkmType = PokemonType::UNDEFINED;
	PokemonRare
		m_pkmRare = PokemonRare::UNDEFINED;
	/*std::array< ,4>*/
};
