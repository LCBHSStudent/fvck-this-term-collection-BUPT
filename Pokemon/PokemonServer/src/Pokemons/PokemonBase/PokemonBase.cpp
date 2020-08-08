#include "PokemonBase.h"

int PokemonBase::LEVEL_UP_EXP[MAX_LEVEL + 1] = {
    0, 0, 1, 2, 3, 5, 7 ,11, 17, 25, 31, 39, 48, 61, 76, 83
};

PropertyCombo PokemonBase::INITIAL_PROPERTY = {
    12, 6, 6, 6
};

PokemonBase::PokemonBase(ARGS_LIST):
    m_name(_name),
    m_curUser(_user),
    m_id(_id),
    m_level(_level),
    m_exp(_exp),
    m_typeID(_typeId),
    m_ATK(_ATK),
    m_HP(_HP),
    m_DEF(_DEF),
    m_SPD(_SPD),
    m_pkmType(PokemonType(_type)),
    m_pkmAttr(PokemonAttribute(_attr)
) {
    m_skills.resize(4);
}

void PokemonBase::printStatus() const {
    qDebug() << "----------------------DISPLAY POKEMON STATUS----------------------";
    qDebug() << "|------------------NAME: " << m_name;
    qDebug() << "|------------------CUR_USER: " << m_curUser;
    qDebug() << "|------------------ATK: " << m_curATK << " / " << m_ATK;
    qDebug() << "|------------------DEF: " << m_curDEF << " / " << m_DEF;
    qDebug() << "|------------------SPD: " << m_curSPD << " / " << m_SPD;
    qDebug() << "|------------------HP: "  << m_curHP  << " / " << m_HP;
    qDebug() << "------------------------DISPLAY FINISHED--------------------------";
}


void PokemonBase::gainExperience(int exp) {
	if ((m_exp + exp) <= m_exp)
		return;
	m_exp += exp;
	int levelUpValue =
		50 * this->m_level * (this->m_level + 1) / 2;
	while (m_level < MAX_LEVEL) {
		if (m_exp >= levelUpValue) {
			this->levelUp();
			levelUpValue += 50 * m_level;
		}
	}
}

