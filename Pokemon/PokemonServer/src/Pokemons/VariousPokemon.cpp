#include "VariousPokemon.h"

//----------------------ATK REFERED------------------------//
HighAtkPkm::HighAtkPkm(ARGS_LIST): PokemonBase(PKM_INIT_LIST) {
    
}

Buff HighAtkPkm::attack(
    PokemonBase&    target,
    QString&        skillName
) {
    return {0, 0};
}

void HighAtkPkm::levelUp() {
    
}

//----------------------HP REFERED-------------------------//
HighHpPkm::HighHpPkm(ARGS_LIST): PokemonBase(PKM_INIT_LIST) {
    
}

Buff HighHpPkm::attack(
    PokemonBase&    target,
    QString&        skillName
) {
    return {0, 0};
}

void HighHpPkm::levelUp() {
    
}

//----------------------DEF REFERED------------------------//
HighDefPkm::HighDefPkm(ARGS_LIST): PokemonBase(PKM_INIT_LIST) {
    
}

Buff HighDefPkm::attack(
    PokemonBase&    target,
    QString&        skillName
) {
    return {0, 0};
}

void HighDefPkm::levelUp() {
    
}


//----------------------SPD REFERED------------------------//
HighSpdPkm::HighSpdPkm(ARGS_LIST): PokemonBase(PKM_INIT_LIST) {
    
}

Buff HighSpdPkm::attack(
    PokemonBase&    target,
    QString&        skillName
) {
    return {0, 0};
}

void HighSpdPkm::levelUp() {
    
}
