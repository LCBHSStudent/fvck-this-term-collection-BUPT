#include "PokemonBase.h"

PokemonBase::PokemonBase(ARGS_LIST):
    m_name(_name),
    m_id(_id),
    m_level(_level),
    m_exp(_exp),
    m_ATK(_ATK),
    m_HP(_HP),
    m_DEF(_DEF),
    m_SPD(_SPD),
    m_pkmType(PokemonType(_type)),
    m_pkmAttr(PokemonAttribute(_attr)) {}

int PokemonBase::LEVEL_UP_EXP[15] = {
    0
};

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

