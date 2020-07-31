#include "UnitTest.h"

void UnitTest::TestPokemonBattle() {
    
}

void UnitTest::TestPokemonProperties() {
    auto pkm = PokemonFactory::CreatePokemon("_server", 1);
    pkm->gainExperience(1);
}
