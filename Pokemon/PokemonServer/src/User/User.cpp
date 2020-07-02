#include "User.h"
#include "../Pokemons/PokemonBase/PokemonBase.h"

User::User() : QObject() {
    
}

User::~User() {
    if(m_selectedPkm != nullptr) {
        delete m_selectedPkm;
    }
}

void User::createBattlePkm(uint32 pkmId) {
    
}
