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
			const uint32 skillIndex
		) override;
	void
		levelUp() override;
};

