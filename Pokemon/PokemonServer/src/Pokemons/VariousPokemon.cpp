#include "VariousPokemon.h"

//----------------------ATK REFERED------------------------//
HighAtkPkm::HighAtkPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_ATTACK)) {}

AttackResult HighAtkPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    return PokemonSkill::useSkillByName(skillName, this, &target);
}

void HighAtkPkm::levelUp() {
    while(m_exp < LEVEL_UP_EXP[m_level]) {
        m_level++;
    }
}

//----------------------HP REFERED-------------------------//
HighHpPkm::HighHpPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_HITPOINT)) {}

AttackResult HighHpPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    return PokemonSkill::useSkillByName(skillName, this, &target);
}

void HighHpPkm::levelUp() {
    
}

//----------------------DEF REFERED------------------------//
HighDefPkm::HighDefPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_DEFENSE)) {}

AttackResult HighDefPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    return PokemonSkill::useSkillByName(skillName, this, &target);
}

void HighDefPkm::levelUp() {
    
}


//----------------------SPD REFERED------------------------//
HighSpdPkm::HighSpdPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_SPEED)) {}

AttackResult HighSpdPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    return PokemonSkill::useSkillByName(skillName, this, &target);
}

void HighSpdPkm::levelUp() {
    
}
