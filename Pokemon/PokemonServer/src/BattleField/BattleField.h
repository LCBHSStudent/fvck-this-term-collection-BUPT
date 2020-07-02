#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <PreCompile.h>
#include "../User/User.h"
#include "../Pokemons/PokemonBase/PokemonBase.h"

class BattleField: public QObject {
    Q_OBJECT
public: 
        BattleField(
            User*           userA,
            User*           userB,
            PokemonBase*    pkmA,
            PokemonBase*    pkmB,
            QObject*        parent = nullptr
        );
    virtual 
        ~BattleField();
    
public FUNCTION:
    void turn(const QString& actionA, const QString& actionB);
    
private:
    void queryBuffList();
    
private RESOURCE:
    std::array<User*, 2>
        m_users = {};
    std::array<PokemonBase*, 2>
        m_pkmList = {};
    QList<Buff>
        m_buffListA = {},
        m_buffListB = {};
};

#endif // BATTLEFIELD_H
