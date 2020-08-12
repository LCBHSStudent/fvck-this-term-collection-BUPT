#include "VariousPokemon.h"
#include "../StorageHelper/StorageHelper.h"

// ------升级函数扩展域 (代码重用)
// ------------为升级函数的可扩展性
// ------------使用时在LevelUp函数内
// ------------声明LEVELUP_PREFIX后操作升级变量
// ------------使用LEVELUP_POSTFIX将变更写入user对应的宝可梦数据库 (废弃，改为调用save2...)
#define LEVELUP_PREFIX \
    int ATK_INC = 0, \
        DEF_INC = 0, \
        HP_INC  = 0, \
        SPD_INC = 0; \
    StorageHelper::Instance().transaction( \
        "SELECT ATK_INC, DEF_INC, HP_INC, SPD_INC\
         FROM `pokemon_info` WHERE PKM_ID=?", \
        [&ATK_INC, &DEF_INC, &HP_INC, &SPD_INC](QSqlQuery& query) { \
            ATK_INC = query.value(0).toInt(); \
            DEF_INC = query.value(1).toInt(); \
            HP_INC  = query.value(2).toInt(); \
            SPD_INC = query.value(3).toInt(); \
        }, \
        this->m_typeID \
    ); \

#define LEVELUP_POSTFIX \
    this->m_ATK += ATK_INC;     \
    this->m_DEF += DEF_INC;     \
    this->m_SPD += SPD_INC;     \
    this->m_HP  += HP_INC;      \

//----------------------ATK REFERED------------------------//
HighAtkPkm::HighAtkPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_ATTACK)) {}

// ------重写攻击型宝可梦的attack函数
// ------主要特性为有概率增幅伤害，并且无视防御型宝可梦的判定效果
/**
 * @brief HighAtkPkm::attack
 * @param target
 * @param skillName
 * @return {AttackResult} 攻击结果
 */
AttackResult HighAtkPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    AttackResult&& result = PokemonSkill::useSkillByName(skillName, this, &target);
    
    JUDGE_OZ(20) {
        if(result.destDeltaHp)
            result.destDeltaHp *= 1.2;
    }
    
    return result;
}

// ------重写攻击型宝可梦的升级函数
// ------主要特性为升级时概率性额外增加1点ATK
/**
 * @brief HighAtkPkm::levelUp
 */
void HighAtkPkm::levelUp() {
    LEVELUP_PREFIX
    JUDGE_OZ(20) {
        ATK_INC++;
    }
    LEVELUP_POSTFIX
}

//----------------------HP REFERED-------------------------//
HighHpPkm::HighHpPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_HITPOINT)) {}

// ------重写耐久型宝可梦的attack函数
// ------主要特性为有概率增幅Buff效果和Buff持续时间
/**
 * @brief HighHpPkm::attack
 * @param target
 * @param skillName
 * @return {AttackResult} 攻击结果
 */
AttackResult HighHpPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    AttackResult&& result = PokemonSkill::useSkillByName(skillName, this, &target);
    
    JUDGE_OZ(10) {
        if (result.selfBuf.buffId == ATK_UP_S ||
            result.selfBuf.buffId == ATK_UP_L ||
            result.selfBuf.buffId == DEF_UP_S ||
            result.selfBuf.buffId == DEF_UP_L ||
            result.selfBuf.buffId == SPD_UP_S ||
            result.selfBuf.buffId == SPD_UP_L 
        ) {
            result.selfBuf.buffId++;
            result.selfBuf.turnCnt++;
        }
    }
    
    return result;
}

// ------重写攻击型宝可梦的升级函数
// ------主要特性为升级时概率性额外增加2点HP
/**
 * @brief HighHpPkm::levelUp
 */
void HighHpPkm::levelUp() {
    LEVELUP_PREFIX
    JUDGE_OZ(25) {
        HP_INC++;
    }
    LEVELUP_POSTFIX
}

//----------------------DEF REFERED------------------------//
HighDefPkm::HighDefPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_DEFENSE)) {}

// ------重写防御型宝可梦的attack函数
// ------主要特性为有概率为对方附加Debuff
/**
 * @brief HighDefPkm::attack
 * @param target
 * @param skillName
 * @return {AttackResult} 攻击结果
 */
AttackResult HighDefPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    AttackResult&& result = PokemonSkill::useSkillByName(skillName, this, &target);
    
    JUDGE_OZ(20) {
        if(!result.destBuf.buffId) {
            JUDGE_OZ(50) {
                JUDGE_OZ(50) {
                    JUDGE_OZ(50) {
                        result.destBuf.buffId = POISONING;
                    } else {
                        result.destBuf.buffId = SPD_DOWN_S;
                    }
                } else {
                    result.destBuf.buffId = DEF_DOWN_S;
                }
            } else {
                result.destBuf.buffId = ATK_DOWN_S;
            }
            
            result.destBuf.turnCnt = 1;
        }
    }
    
    return result;
}

// ------重写攻击型宝可梦的升级函数
// ------主要特性为升级时概率性额外增加1点DEF
/**
 * @brief HighDefPkm::levelUp
 */
void HighDefPkm::levelUp() {
    LEVELUP_PREFIX
    JUDGE_OZ(20) {
        DEF_INC++;
    }
    LEVELUP_POSTFIX
}


//----------------------SPD REFERED------------------------//
HighSpdPkm::HighSpdPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_SPEED)) {}

// ------重写速度型宝可梦的attack函数
// ------主要特性为有概率获得免伤buff
/**
 * @brief HighSpdPkm::attack
 * @param target
 * @param skillName
 * @return {AttackResult} 攻击结果
 */
AttackResult HighSpdPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    AttackResult&& result = PokemonSkill::useSkillByName(skillName, this, &target);
    
    JUDGE_OZ(10) {
        if(!result.selfBuf.buffId) {
            result.selfBuf.buffId   = DAMAGE_HALF;
            result.selfBuf.turnCnt  = 1;
        }
    }
    return result;
}

// ------重写攻击型宝可梦的升级函数
// ------主要特性为升级时小概率额外增加1点SPD
/**
 * @brief HighSpdPkm::levelUp
 */
void HighSpdPkm::levelUp() {
    LEVELUP_PREFIX
    JUDGE_OZ(20) {
        SPD_INC++;
    }
    LEVELUP_POSTFIX
}
