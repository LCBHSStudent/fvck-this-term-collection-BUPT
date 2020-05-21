#pragma once

#include "PokemonBase/PokemonBase.h"

class HighAtkPkm: public PokemonBase {
public:	
	explicit
		HighAtkPkm(const std::string_view name);
	virtual
		~HighAtkPkm() = default;
public FUNCTION:	
	uint32
		attack(
            PokemonBase&    target,
            uint32          skillIndex = 0
		) override;
	void
		levelUp() override;
};

