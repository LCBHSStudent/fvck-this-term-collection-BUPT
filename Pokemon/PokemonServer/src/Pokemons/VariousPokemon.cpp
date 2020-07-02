#include "VariousPokemon.h"

//----------------------ATK REFERED------------------------//
HighAtkPkm::HighAtkPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_ATTACK)) {}

BuffResult HighAtkPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    PokemonSkill::useSkillByName(skillName, this, &target);
    
    return BuffResult();
}

void HighAtkPkm::levelUp() {
    
}

//----------------------HP REFERED-------------------------//
HighHpPkm::HighHpPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_HITPOINT)) {}

BuffResult HighHpPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    PokemonSkill::useSkillByName(skillName, this, &target);
    
    return BuffResult();
}

void HighHpPkm::levelUp() {
    
}

//----------------------DEF REFERED------------------------//
HighDefPkm::HighDefPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_DEFENSE)) {}

BuffResult HighDefPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    PokemonSkill::useSkillByName(skillName, this, &target);
    
    return BuffResult();
}

void HighDefPkm::levelUp() {
    
}


//----------------------SPD REFERED------------------------//
HighSpdPkm::HighSpdPkm(ARGS_LIST): 
    PokemonBase(PKM_INIT_LIST(PokemonType::HIGH_SPEED)) {}

BuffResult HighSpdPkm::attack(
    PokemonBase&    target,
    const QString&  skillName
) {
    PokemonSkill::useSkillByName(skillName, this, &target);
    
    return BuffResult();
}

void HighSpdPkm::levelUp() {
    
}
