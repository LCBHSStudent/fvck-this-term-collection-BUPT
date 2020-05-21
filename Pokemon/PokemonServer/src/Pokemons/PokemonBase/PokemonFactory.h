#ifndef POKEMONFACTORY_H
#define POKEMONFACTORY_H

#include <PreCompile.h>
#include "PokemonBase.h"

#define CreatePokemon() PokemonBase*

class PokemonFactory {
public:
    PokemonFactory() = delete;
    
//    static std::unique_ptr<PokemonFactory>
    
};

#endif // POKEMONFACTORY_H
