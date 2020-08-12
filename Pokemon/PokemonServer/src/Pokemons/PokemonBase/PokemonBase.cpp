#include "PokemonBase.h"

// 升级经验
int PokemonBase::LEVEL_UP_EXP[MAX_LEVEL-1] = {
    1, 2, 3, 5, 7 ,11, 17, 25, 31, 39, 48, 61, 76, 83
};

// 初始属性值 (废弃，被sql插入宝可梦数据时的default value代替)
PropertyCombo PokemonBase::INITIAL_PROPERTY = {
    12, 6, 6, 6
};

/**
 * @brief PokemonBase::PokemonBase
 *        宝可梦基类构造函数
 * @param _name     {QString}
 * @param _user     {QString}
 * @param _id       {int}
 * @param _typeId   {int}
 * @param _level    {int}
 * @param _exp      {int}
 * @param _ATK      {int}
 * @param _HP       {int}
 * @param _DEF      {int}
 * @param _SPD      {int}
 * @param _attr     {enum}
 * @param _type     {enum}
 */
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

/**
 * @brief PokemonBase::printStatus
 *        打印宝可梦信息
 */
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

/**
 * @brief PokemonBase::save2LocalStorage
 *        存储宝可梦信息到数据库
 */
void PokemonBase::save2LocalStorage() const {
    StorageHelper::Instance().transaction(
        "UPDATE `user_" + m_curUser + "` SET \
         PKM_LEVEL=?, PKM_EXP=?, PKM_ATK=?, PKM_DEF=?, PKM_HP=?, PKM_SPD=? \
         WHERE PKM_ID=?",
        StorageHelper::DEFAULT_FUNC,
        m_level, m_exp, m_ATK, m_DEF, m_HP, m_SPD, m_id
    );
}

/**
 * @brief PokemonBase::gainExperience
 *        获取经验值
 * @param exp   {int} 经验值变化量
 */
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

