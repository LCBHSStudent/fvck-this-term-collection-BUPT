#include "HighAtkPkm.h"

HighAtkPkm::HighAtkPkm(const std::string_view name) {
	m_name = name;
}

uint32 HighAtkPkm::attack(
        const uint32        skillIndex,
        const PokemonBase*  target
) {
	return 0;
}

void HighAtkPkm::levelUp() {
    
}
