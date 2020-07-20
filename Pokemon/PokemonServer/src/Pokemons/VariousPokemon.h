#pragma once

#include "PokemonBase/PokemonBase.h"
#include "PokemonBase/PokemonSkill.h"

#define PKM_INIT_LIST(_TYPE) \
    _name, _id, _level, _exp, _ATK, _HP, _DEF, _SPD, _attr, _TYPE



class HighAtkPkm: public PokemonBase {
    friend class PokemonSkill;
public:	
		HighAtkPkm(DEF_ARGS_LIST);
	virtual
		~HighAtkPkm() = default;
public FUNCTION:	
	AttackResult
		attack(
            PokemonBase&    target,
            const QString&  skillName
		) override;
	void
		levelUp() override;
};


class HighHpPkm: public PokemonBase {
    friend class PokemonSkill;
public:	
	explicit
		HighHpPkm(DEF_ARGS_LIST);
	virtual
		~HighHpPkm() = default;
public FUNCTION:	
	AttackResult
        attack(
            PokemonBase&    target,
            const QString&  skillName
        ) override;
	void
		levelUp() override;
};



class HighDefPkm: public PokemonBase {
    friend class PokemonSkill;
public:	
	explicit
		HighDefPkm(DEF_ARGS_LIST);
	virtual
		~HighDefPkm() = default;
public FUNCTION:	
	AttackResult
		attack(
            PokemonBase&    target,
            const QString&  skillName
		) override;
	void
		levelUp() override;
};


class HighSpdPkm: public PokemonBase {
    friend class PokemonSkill;
public:	
	explicit
		HighSpdPkm(DEF_ARGS_LIST);
	virtual
		~HighSpdPkm() = default;
public FUNCTION:	
	AttackResult
		attack(
            PokemonBase&    target,
            const QString&  skillName
		) override;
	void
		levelUp() override;
};
