#pragma once

#include "PokemonBase/PokemonBase.h"
    
#define PKM_INIT_LIST \
    _name, _id, _level, _exp, _ATK, _HP, _DEF, _SPD, _type, _attr



class HighAtkPkm: public PokemonBase {
public:	
		HighAtkPkm(ARGS_LIST);
	virtual
		~HighAtkPkm() = default;
public FUNCTION:	
	uint32
		attack(
            PokemonBase&    target,
            QString&        skillName
		) override;
	void
		levelUp() override;
};


class HighHpPkm: public PokemonBase {
public:	
	explicit
		HighHpPkm(ARGS_LIST);
	virtual
		~HighHpPkm() = default;
public FUNCTION:	
	uint32
		attack(
            PokemonBase&    target,
            QString&        skillName
		) override;
	void
		levelUp() override;
};



class HighDefPkm: public PokemonBase {
public:	
	explicit
		HighDefPkm(ARGS_LIST);
	virtual
		~HighDefPkm() = default;
public FUNCTION:	
	uint32
		attack(
            PokemonBase&    target,
            QString&        skillName
		) override;
	void
		levelUp() override;
};


class HighSpdPkm: public PokemonBase {
public:	
	explicit
		HighSpdPkm(ARGS_LIST);
	virtual
		~HighSpdPkm() = default;
public FUNCTION:	
	uint32
		attack(
            PokemonBase&    target,
            QString&        skillName
		) override;
	void
		levelUp() override;
};
