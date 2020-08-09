#include "PokemonBase.h"

int PokemonBase::LEVEL_UP_EXP[MAX_LEVEL-1] = {
    1, 2, 3, 5, 7 ,11, 17, 25, 31, 39, 48, 61, 76, 83
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

void PokemonBase::save2LocalStorage() const {
    StorageHelper::Instance().transaction(
        "UPDATE `user_" + m_curUser + "` SET \
         PKM_LEVEL=?, PKM_EXP=?, PKM_ATK=?, PKM_DEF=?, PKM_HP=?, PKM_SPD=? \
         WHERE PKM_ID=?",
        StorageHelper::DEFAULT_FUNC,
        m_level, m_exp, m_ATK, m_DEF, m_HP, m_SPD, m_id
    );
}

void PokemonBase::gainExperience(int exp) {
    if ((m_exp + exp) <= m_exp) {
		return;
    }
	m_exp += exp;
	
    while (m_level < MAX_LEVEL && m_exp >= LEVEL_UP_EXP[m_level-1]) {
        this->levelUp();
		this->m_exp -= LEVEL_UP_EXP[m_level-1];
        this->m_level++;
	}
	this->save2LocalStorage();
}

