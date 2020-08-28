#include "UnitTest.h"

void UnitTest::TestPokemonBattle() {
    
}

void UnitTest::TestPokemonProperties() {
    auto pkm = PokemonFactory::CreatePokemon("_server", 1);
    
    pkm->printStatus();
    
    
    pkm->gainExperience(1);
}
