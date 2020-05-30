#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <PreCompile.h>
#include "../User/User.h"
#include "../Pokemons/PokemonBase/PokemonBase.h"

class BattleField: public QObject {
    Q_OBJECT
public:
    explicit 
        BattleField(
            User* userA,
            User* userB,
            QObject* parent = nullptr
        ):  QObject(parent) {}
        
        
public FUNCTION:
    
        
private RESOURCE:
    
};

#endif // BATTLEFIELD_H
