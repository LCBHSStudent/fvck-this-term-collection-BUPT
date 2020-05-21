#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <PreCompile.h>
#include "../User/User.h"
#include "../Pokemons/PokemonBase/PokemonBase.h"

class BattleField: public QObject {
    Q_OBJECT
public:
    explicit 
        BattleField(QObject* parent = nullptr):
            QObject(parent) {}
};

#endif // BATTLEFIELD_H
