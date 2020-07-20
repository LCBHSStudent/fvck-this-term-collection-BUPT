#include "BattleField.h"

BattleField::BattleField(
    User*           userA,
    User*           userB,
    PokemonBase*    pkmA,
    PokemonBase*    pkmB,
    QObject*        parent
):  QObject(parent) {
    // 初始化对战环境啊嗯
    {
        m_users = {userA, userB};
        m_pkmList = {pkmA, pkmB};
    }
    // 初始化宝可梦战斗属性
    {
        pkmA->set_curHP(pkmA->get_HP());
        pkmA->set_curATK(pkmA->get_ATK());
        pkmA->set_curDEF(pkmA->get_DEF());
        pkmA->set_curSPD(pkmA->get_SPD());
        
        pkmB->set_curHP(pkmA->get_HP());
        pkmB->set_curATK(pkmA->get_ATK());
        pkmB->set_curDEF(pkmA->get_DEF());
        pkmB->set_curSPD(pkmA->get_SPD());
    }
}

BattleField::~BattleField() {
    
}

void BattleField::turn(const QString& actionA, const QString& actionB) {
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
            auto resA = pkmA.attack(pkmB, actionA);
            auto resB = pkmB.attack(pkmA, actionB);
            
            (void)resA, (void)resB;
        } 
        else if (spdA < spdB) {
            auto resB = pkmB.attack(pkmA, actionB);
            auto resA = pkmA.attack(pkmB, actionA);
            
            (void)resA, (void)resB;
        }
    }
    
}

void BattleField::queryBuffList() {
    for(auto buff: m_buffListA) {
        switch (buff.buffId) {
        case 0:
            break;
        default:
            break;
        }
    }
    for(auto buff: m_buffListA) {
        switch (buff.buffId) {
        case 0:
            break;
        default:
            break;
        }
    }
}
