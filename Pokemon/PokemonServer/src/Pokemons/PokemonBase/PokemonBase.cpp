#include "PokemonBase.h"

void PokemonBase::gainExperience(uint32 exp) {
	if ((m_exp + exp) <= m_exp)
		return;
	m_exp += exp;
	uint32 levelUpValue =
		50 * this->m_level * (this->m_level + 1) / 2;
	while (m_level < MAX_LEVEL) {
		if (m_exp >= levelUpValue) {
			this->levelUp();
			levelUpValue += 50 * m_level;
		}
	}
}

