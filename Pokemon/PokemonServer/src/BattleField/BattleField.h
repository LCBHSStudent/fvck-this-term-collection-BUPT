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
    
public RESOURCE:
    enum BalanceType {
        A_TO_B = 0,
        B_TO_A
    };
        
public FUNCTION:
    void 
        turn(const QString& actionA, const QString& actionB);
    
private FUNCTION:
    void 
        queryBuffList();
    void
        handleResult(AttackResult& result, BalanceType type);
    
signals:
    void
        sigBattleFinished(User* wonner, User* loser);
    
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
