#include "BattleField.h"

BattleField::BattleField(
    User*           userA,
    User*           userB,
    PokemonBase*    pkmA,
    PokemonBase*    pkmB,
    QObject*        parent
):  QObject(parent) {
    m_users = {userA, userB};
    m_pkmList = {pkmA, pkmB};
}

BattleField::~BattleField() {
    
}

void BattleField::turn(QString& actionA, QString& actionB) {
    PokemonBase& pkmA = *m_pkmList[0];
    PokemonBase& pkmB = *m_pkmList[1];
    queryBuffList();
    
    {
        auto spdA = pkmA.get_curSPD();
        auto spdB = pkmB.get_curSPD();
        if(spdA == spdB) {
            spdA += pow(-1, spdA % 2);
        }
        
        if(spdA > spdB) {
            pkmA.attack(pkmB, actionA);
            pkmB.attack(pkmA, actionB);
        } else if (spdA < spdB) {
            pkmB.attack(pkmA, actionB);
            pkmA.attack(pkmB, actionA);
        }
    }
    
}

void BattleField::queryBuffList() {
    for(auto buff: m_buffList) {
        switch (buff) {
        case 0:
            break;
        default:
            break;
        }
    }
}
