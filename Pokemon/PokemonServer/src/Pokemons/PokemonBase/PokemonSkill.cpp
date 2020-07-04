#include "PokemonSkill.h"
#include "PokemonBase.h"

#define REGISTER_SKILL REGISTER_METHOD

// ----------DEFINATIONS FOR CONVIENCE---------- //

#define JUDGE_ATTR(_ATTR) \
    (dest->m_pkmAttr == PokemonBase::PokemonAttribute::_ATTR) \

#define ATK_DEBUG(_SKILL) \
    qDebug()  << user->m_name \
              + "使用" #_SKILL "攻击了" \
              + dest->m_name + "\n" \

#define RAND_OZ \
    PokemonSkill::s_distr(PokemonSkill::s_engine) \

#define JUDGE_PKM_TYPE(_pkm, _type) \
    if(_pkm->m_pkmType == PokemonBase::PokemonType::_type) \

#define JUDGE_OZ(_standard) \
    if(s_distr(s_engine) <= get_percent(_standard)) \

// ---------GLOBAL VARIABLES---------- //
QHash<QString, PokemonSkill::SkillFunc> 
    PokemonSkill::s_skillMap = {};

std::random_device
    PokemonSkill::s_rdev;
std::default_random_engine 
    PokemonSkill::s_engine(s_rdev());
std::uniform_real_distribution<float> 
    PokemonSkill::s_distr(0.0f, 1.0f);
constexpr auto get_percent = [](int percentVal) {
    return static_cast<float>(percentVal) / 100.0f;};
// ----------------------------------- //



// ------------DEFINATION OF SKILLS------------ //
REGISTER_SKILL(JJJJ) {
    ATK_DEBUG(普通攻击);
    qDebug() << &user->m_curATK << &dest->m_curDEF;
    int damage = user->m_curATK - dest->m_curDEF;
    if(damage > 0) {
        JUDGE_PKM_TYPE(dest, HIGH_DEFENSE) {
            JUDGE_OZ(50) {
#ifdef DEBUG_FLAG
                qDebug() << dest->m_name + "由于防御型宝可梦特性，有概率抵消一半伤害\n";
#endif 
                damage /= 2;
            }
        }
        JUDGE_PKM_TYPE(user, HIGH_ATTACK) {
            damage *= 1.5;
        }
        dest->m_curHP -= damage;
    }
}

REGISTER_SKILL(FireBall) {
    ATK_DEBUG(火球);
    //---计算伤害---//
    
    if JUDGE_ATTR(GRASS) {
        
    } else if JUDGE_ATTR(WATER) {
        
    } else {
        
    }
    
    //------生成buff-----//
    //--------效果-------//
    //return x (buff id)//
    
}

REGISTER_SKILL(GreassLeaf) {
    ATK_DEBUG(叶刃);
    //---计算伤害---//
    
    if JUDGE_ATTR(GRASS) {
        
    } else if JUDGE_ATTR(WATER) {
        
    } else {
        
    }
    
    //------生成buff-----//
    //--------效果-------//
    //return x (buff id)//
}

REGISTER_SKILL(WaterBullet) {
    ATK_DEBUG(水弹);
    //---计算伤害---//
    
    if JUDGE_ATTR(GRASS) {
        
    } else if JUDGE_ATTR(WATER) {
        
    } else {
        
    }
    
    //------生成buff-----//
    //--------效果-------//
    //return x (buff id)//
}

REGISTER_SKILL(WindBreath) {
    ATK_DEBUG(风息);
    //---计算伤害---//
    
    if JUDGE_ATTR(GRASS) {
        
    } else if JUDGE_ATTR(WATER) {
        
    } else {
        
    }
    
    //------生成buff-----//
    //--------效果-------//
    //return x (buff id)//
}

// ------------------------------------- //
