#include <PreCompile.h>

#include "Pokemons/TestPokemonFunction.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    TestCreatePokemon();
    TestPokemonSkillFuncAddr();
    
    return app.exec();
}
