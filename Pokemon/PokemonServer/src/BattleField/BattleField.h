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
    void turn(QString& actionA, QString& actionB);
    
public RESOURCE:
    QList<int>
        m_buffList = {};
    
private:
    void queryBuffList();
    
private RESOURCE:
    std::array<User*, 2>
        m_users = {};
    std::array<PokemonBase*, 2>
        m_pkmList = {};
};

#endif // BATTLEFIELD_H
