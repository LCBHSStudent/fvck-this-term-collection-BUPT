#ifndef UNITTEST_H
#define UNITTEST_H

#include <PreCompile.h>
#include <Reflect.hpp>

#define UNIT_TEST_FLAG

#include "Pokemons/PokemonBase/PokemonBase.h"
#include "Pokemons/PokemonBase/PokemonSkill.h"
#include "Pokemons/PokemonBase/PokemonFactory.h"

#include "User/User.h"
#include "BattleField/BattleField.h"
#include "NetworkHelper/NetworkHelper.h"
#include "ServerBackend/ServerBackend.h"
#include "StorageHelper/StorageHelper.h"

class UnitTest {
public:
    // TEST FUNCTION FOR STAGE 1
    static void
        TestPokemonProperties();
    static void
        TestPokemonLevelUp();
    static void
        TestPokemonBattle();
    
    
};

#endif // UNITTEST_H
