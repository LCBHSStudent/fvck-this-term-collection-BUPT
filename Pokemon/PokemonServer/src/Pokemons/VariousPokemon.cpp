#include "VariousPokemon.h"

HighAtkPkm::HighAtkPkm(
    ARGS_LIST
):PokemonBase(PKM_INIT_LIST) {
    
}

uint32 HighAtkPkm::attack(
    PokemonBase&    target,
    uint32          skillIndex
) {
    return 0;
}

void HighAtkPkm::levelUp() {
    
}
