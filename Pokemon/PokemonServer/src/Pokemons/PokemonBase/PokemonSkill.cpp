#include "PokemonSkill.h"
#include "PokemonBase.h"

#define REGISTER_SKILL REGISTER_METHOD

#define JUDGE_ATTR(_ATTR) \
    (dest->m_pkmAttr == PokemonBase::PokemonAttribute::_ATTR)

#define ATK_DEBUG(_SKILL) \
    std::cout << user->m_name << "使用" #_SKILL "攻击了" << dest->m_name << "\n"

QHash<QString, PokemonSkill::SkillFunc> PokemonSkill::s_skillMap = {};

REGISTER_SKILL(FireBall) {
    ATK_DEBUG("火球");
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
    ATK_DEBUG("叶刃");
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
    ATK_DEBUG("水弹");
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
    ATK_DEBUG("风息");
    //---计算伤害---//
    
    if JUDGE_ATTR(GRASS) {
        
    } else if JUDGE_ATTR(WATER) {
        
    } else {
        
    }
    
    //------生成buff-----//
    //--------效果-------//
    //return x (buff id)//
}
