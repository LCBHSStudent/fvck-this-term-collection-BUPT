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
        
        pkmB->set_curHP(pkmB->get_HP());
        pkmB->set_curATK(pkmB->get_ATK());
        pkmB->set_curDEF(pkmB->get_DEF());
        pkmB->set_curSPD(pkmB->get_SPD());
    }
    qDebug() << "[BATTLE FIELD]: CREATED A NEW BATTLE FIELD";
}

BattleField::~BattleField() {
    delete m_pkmList[0];
    delete m_pkmList[1];
    m_pkmList = {nullptr, nullptr};
    
    qDebug() << "[BATTLE FIELD]: DESTROYED A BATTLE FIELD";
}

void BattleField::setAction(const QString& action, int index) {
    if (index > 1 || index < 0) {
        return;
    }
    m_actions[index] = action;
    if (m_actions[0].length() != 0 &&
        m_actions[1].length() != 0
    ) {
        turn(m_actions[0], m_actions[1]);
    } 
}


void BattleField::turn(const QString& actionA, const QString& actionB) {
    PokemonBase& pkmA = *m_pkmList[0];
    PokemonBase& pkmB = *m_pkmList[1];
    
    // ------ 暂时不考虑平局
    static const auto chechBattleIsFinished = [&pkmA, &pkmB, this] {
        if (pkmA.get_curHP() <= 0) {
            emit this->sigBattleFinished(m_users[1], m_users[0]);
        } else if (pkmB.get_curHP() <= 0) {
            emit this->sigBattleFinished(m_users[0], m_users[1]);
        }
    };
    
    // ------ main logic 
    {
        auto spdA = pkmA.get_curSPD();
        auto spdB = pkmB.get_curSPD();
        if(spdA == spdB) {
            spdA += pow(-1, spdA % 2);
        }
        
        if(spdA > spdB) {
            auto resA = pkmA.attack(pkmB, actionA);
            handleResult(resA, A_TO_B);
            {
                pkmA.set_curHP(
                    std::min(pkmA.get_curHP() - resA.selfDeltaHp, pkmA.get_HP()));
                pkmA.set_curHP(
                    std::min(pkmB.get_curHP() - resA.destDeltaHp, pkmB.get_HP()));
            }
            chechBattleIsFinished();
            
            auto resB = pkmB.attack(pkmA, actionB);
            handleResult(resB, B_TO_A);
            {
                pkmA.set_curHP(
                    std::min(pkmA.get_curHP() - resB.destDeltaHp, pkmA.get_HP()));
                pkmA.set_curHP(
                    std::min(pkmB.get_curHP() - resB.selfDeltaHp, pkmB.get_HP()));
            }
            chechBattleIsFinished();
        } 
        else if (spdA < spdB) {
            auto resB = pkmB.attack(pkmA, actionB);
            handleResult(resB, B_TO_A);
            {
                pkmA.set_curHP(
                    std::min(pkmA.get_curHP() - resB.destDeltaHp, pkmA.get_HP()));
                pkmA.set_curHP(
                    std::min(pkmB.get_curHP() - resB.selfDeltaHp, pkmB.get_HP()));
            }
            chechBattleIsFinished();
                    
            auto resA = pkmA.attack(pkmB, actionA);
            handleResult(resA, A_TO_B);
            {
                pkmA.set_curHP(
                    std::min(pkmA.get_curHP() - resA.selfDeltaHp, pkmA.get_HP()));
                pkmA.set_curHP(
                    std::min(pkmB.get_curHP() - resA.destDeltaHp, pkmB.get_HP()));
            }
            chechBattleIsFinished();
        }
    }
    
    queryBuffList();
    chechBattleIsFinished();
}


// TODO: 增加LOG
void BattleField::queryBuffList() {
    PokemonBase& pkmA = *m_pkmList[0];
    PokemonBase& pkmB = *m_pkmList[1];
    {
        pkmA.set_curATK(pkmA.get_ATK());
        pkmA.set_curDEF(pkmA.get_DEF());
        pkmA.set_curSPD(pkmA.get_SPD());
        
        pkmB.set_curATK(pkmB.get_ATK());
        pkmB.set_curDEF(pkmB.get_DEF());
        pkmB.set_curSPD(pkmB.get_SPD());
    }
    
    // -------- lambda for convenience
    static const auto handleBuffList = [](PokemonBase& pkm, QList<Buff>& buffList) {
        // ------ 采用后序遍历应该能快一点?
        for (int i = buffList.size() - 1; i >= 0; i--) {
            Buff& buff = buffList[i];
            
            switch (buff.buffId) {
            // ------ properties up
            case PokemonBase::BuffType::ATK_UP_S:
                pkm.set_curATK(pkm.get_curATK() * 1.1);
                break;
            case PokemonBase::BuffType::ATK_UP_M:
                pkm.set_curATK(pkm.get_curATK() * 1.2);
                break;
            case PokemonBase::BuffType::ATK_UP_L:
                pkm.set_curATK(pkm.get_curATK() * 1.3);
                break;
            case PokemonBase::BuffType::DEF_UP_S:
                pkm.set_curDEF(pkm.get_curDEF() * 1.1);
                break;
            case PokemonBase::BuffType::DEF_UP_M:
                pkm.set_curDEF(pkm.get_curDEF() * 1.2);
                break;
            case PokemonBase::BuffType::DEF_UP_L:
                pkm.set_curDEF(pkm.get_curDEF() * 1.3);
                break;
            case PokemonBase::BuffType::SPD_UP_S:
                pkm.set_curSPD(pkm.get_curSPD() * 1.1);
                break;
            case PokemonBase::BuffType::SPD_UP_M:
                pkm.set_curSPD(pkm.get_curSPD() * 1.2);
                break;
            case PokemonBase::BuffType::SPD_UP_L:
                pkm.set_curSPD(pkm.get_curSPD() * 1.3);
                break;
                
            // ------ properties down
            case PokemonBase::BuffType::ATK_DOWN_S:
                pkm.set_curATK(pkm.get_curATK() * 0.9);
                break;
            case PokemonBase::BuffType::ATK_DOWN_M:
                pkm.set_curATK(pkm.get_curATK() * 0.8);
                break;
            case PokemonBase::BuffType::ATK_DOWN_L:
                pkm.set_curATK(pkm.get_curATK() * 0.7);
                break;
            case PokemonBase::BuffType::DEF_DOWN_S:
                pkm.set_curDEF(pkm.get_curDEF() * 0.9);
                break;
            case PokemonBase::BuffType::DEF_DOWN_M:
                pkm.set_curDEF(pkm.get_curDEF() * 0.8);
                break;
            case PokemonBase::BuffType::DEF_DOWN_L:
                pkm.set_curDEF(pkm.get_curDEF() * 0.7);
                break;
            case PokemonBase::BuffType::SPD_DOWN_S:
                pkm.set_curSPD(pkm.get_curSPD() * 0.9);
                break;
            case PokemonBase::BuffType::SPD_DOWN_M:
                pkm.set_curSPD(pkm.get_curSPD() * 0.8);
                break;
            case PokemonBase::BuffType::SPD_DOWN_L:
                pkm.set_curSPD(pkm.get_curSPD() * 0.7);
                break;
                
                
            // ------ recover/suffer some damage
            case PokemonBase::BuffType::REVIVE:
                pkm.set_curHP(
                    std::min(
                        static_cast<int>(pkm.get_HP() * 0.15 + pkm.get_curHP()),
                        pkm.get_HP()
                    )
                ); break;
                
            case PokemonBase::BuffType::POISONING:
                pkm.set_curHP(
                    pkm.get_curHP() - static_cast<int>(pkm.get_HP() * 0.15)
                ); break;
                
            default:
                break;
            }
            
            if (--buff.turnCnt <= 0) {
                buffList.removeAt(i);
            }
        }
    };
    
    // -------- logic code body
    handleBuffList(pkmA, m_buffListA);
    handleBuffList(pkmB, m_buffListB);
    
}

void BattleField::handleResult(
    AttackResult&   result,
    BalanceType     type
) {
    PokemonBase& pkmA = *m_pkmList[0];
    PokemonBase& pkmB = *m_pkmList[1];
    
    // ------------ lambda for convenience 
    static const auto handleDest = [&result](
        PokemonBase&    from,
        QList<Buff>&    fromBuf,
        PokemonBase&    dest,
        QList<Buff>&    destBuf
    ) {
        // ------------ handle dest pokemon buff
        switch (result.destBuf.buffId) {
        // ------ properties up
        case PokemonBase::BuffType::ATK_UP_S:
            dest.set_curATK(dest.get_curATK() * 1.1);
            break;
        case PokemonBase::BuffType::ATK_UP_M:
            dest.set_curATK(dest.get_curATK() * 1.2);
            break;
        case PokemonBase::BuffType::ATK_UP_L:
            dest.set_curATK(dest.get_curATK() * 1.3);
            break;
        case PokemonBase::BuffType::DEF_UP_S:
            dest.set_curDEF(dest.get_curDEF() * 1.1);
            break;
        case PokemonBase::BuffType::DEF_UP_M:
            dest.set_curDEF(dest.get_curDEF() * 1.2);
            break;
        case PokemonBase::BuffType::DEF_UP_L:
            dest.set_curDEF(dest.get_curDEF() * 1.3);
            break;
        case PokemonBase::BuffType::SPD_UP_S:
            dest.set_curSPD(dest.get_curSPD() * 1.1);
            break;
        case PokemonBase::BuffType::SPD_UP_M:
            dest.set_curSPD(dest.get_curSPD() * 1.2);
            break;
        case PokemonBase::BuffType::SPD_UP_L:
            dest.set_curSPD(dest.get_curSPD() * 1.3);
            break;
            
        // ------ properties down
        case PokemonBase::BuffType::ATK_DOWN_S:
            dest.set_curATK(dest.get_curATK() * 0.9);
            break;
        case PokemonBase::BuffType::ATK_DOWN_M:
            dest.set_curATK(dest.get_curATK() * 0.8);
            break;
        case PokemonBase::BuffType::ATK_DOWN_L:
            dest.set_curATK(dest.get_curATK() * 0.7);
            break;
        case PokemonBase::BuffType::DEF_DOWN_S:
            dest.set_curDEF(dest.get_curDEF() * 0.9);
            break;
        case PokemonBase::BuffType::DEF_DOWN_M:
            dest.set_curDEF(dest.get_curDEF() * 0.8);
            break;
        case PokemonBase::BuffType::DEF_DOWN_L:
            dest.set_curDEF(dest.get_curDEF() * 0.7);
            break;
        case PokemonBase::BuffType::SPD_DOWN_S:
            dest.set_curSPD(dest.get_curSPD() * 0.9);
            break;
        case PokemonBase::BuffType::SPD_DOWN_M:
            dest.set_curSPD(dest.get_curSPD() * 0.8);
            break;
        case PokemonBase::BuffType::SPD_DOWN_L:
            dest.set_curSPD(dest.get_curSPD() * 0.7);
            break;
            
        default:
            break;
        }
        
        for(auto& buff: destBuf) {
            // ------damage resisit
            if (buff.buffId == PokemonBase::BuffType::DAMAGE_HALF) {
                result.destDeltaHp *= 0.5;
            }
            else if (buff.buffId == PokemonBase::BuffType::DAMAGE_INVALID) {
                result.destDeltaHp = 0;
            }
        }
        
        // ------------ handle from pokemon buff
        switch (result.selfBuf.buffId) {
        // ------ properties up
        case PokemonBase::BuffType::ATK_UP_S:
            from.set_curATK(from.get_curATK() * 1.1);
            break;
        case PokemonBase::BuffType::ATK_UP_M:
            from.set_curATK(from.get_curATK() * 1.2);
            break;
        case PokemonBase::BuffType::ATK_UP_L:
            from.set_curATK(from.get_curATK() * 1.3);
            break;
        case PokemonBase::BuffType::DEF_UP_S:
            from.set_curDEF(from.get_curDEF() * 1.1);
            break;
        case PokemonBase::BuffType::DEF_UP_M:
            from.set_curDEF(from.get_curDEF() * 1.2);
            break;
        case PokemonBase::BuffType::DEF_UP_L:
            from.set_curDEF(from.get_curDEF() * 1.3);
            break;
        case PokemonBase::BuffType::SPD_UP_S:
            from.set_curSPD(from.get_curSPD() * 1.1);
            break;
        case PokemonBase::BuffType::SPD_UP_M:
            from.set_curSPD(from.get_curSPD() * 1.2);
            break;
        case PokemonBase::BuffType::SPD_UP_L:
            from.set_curSPD(from.get_curSPD() * 1.3);
            break;
            
        // ------ properties down
        case PokemonBase::BuffType::ATK_DOWN_S:
            from.set_curATK(from.get_curATK() * 0.9);
            break;
        case PokemonBase::BuffType::ATK_DOWN_M:
            from.set_curATK(from.get_curATK() * 0.8);
            break;
        case PokemonBase::BuffType::ATK_DOWN_L:
            from.set_curATK(from.get_curATK() * 0.7);
            break;
        case PokemonBase::BuffType::DEF_DOWN_S:
            from.set_curDEF(from.get_curDEF() * 0.9);
            break;
        case PokemonBase::BuffType::DEF_DOWN_M:
            from.set_curDEF(from.get_curDEF() * 0.8);
            break;
        case PokemonBase::BuffType::DEF_DOWN_L:
            from.set_curDEF(from.get_curDEF() * 0.7);
            break;
        case PokemonBase::BuffType::SPD_DOWN_S:
            from.set_curSPD(from.get_curSPD() * 0.9);
            break;
        case PokemonBase::BuffType::SPD_DOWN_M:
            from.set_curSPD(from.get_curSPD() * 0.8);
            break;
        case PokemonBase::BuffType::SPD_DOWN_L:
            from.set_curSPD(from.get_curSPD() * 0.7);
            break;
            
        default:
            break;
        }
        
        for (auto& buff: fromBuf) {
            // ------ skip action
            if (buff.buffId == PokemonBase::BuffType::SLIENT    ||
                buff.buffId == PokemonBase::BuffType::PALSYING  ||
                buff.buffId == PokemonBase::BuffType::SLEEPING
            ) {
                result = AttackResult();
                return;
            }
            
            // ------damage enhance
            else if (buff.buffId == PokemonBase::BuffType::SKILL_DAMAGE_UP) {
                result.destDeltaHp *= 1.2;
            }
            else if (buff.buffId == PokemonBase::BuffType::SKILL_DAMAGE_DOUBLE) {
                result.destDeltaHp *= 2;
            }
        }
    };
    
    
    // ------ logic body
    if (type == A_TO_B) {
        m_buffListA.push_back(result.selfBuf);
        m_buffListB.push_back(result.destBuf);
        
        handleDest(pkmA, m_buffListA, pkmB, m_buffListB);
        
    } else if(type == B_TO_A) {
        m_buffListB.push_back(result.selfBuf);
        m_buffListA.push_back(result.destBuf);
        
        handleDest(pkmB, m_buffListB, pkmA, m_buffListA);
        
    }
    
}
