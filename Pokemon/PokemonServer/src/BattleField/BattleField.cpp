#include "BattleField.h"

// ----------------生成回合信息并emit-------------------- //
#define EMIT_TURN_INFO(_res_, _action_, _type_) \
{                                                       \
    TurnInfo turnInfo = {};                             \
    turnInfo.selfBuff       = _res_.selfBuf;            \
    turnInfo.destBuff       = _res_.destBuf;            \
    turnInfo.skillName      = _action_;                 \
    turnInfo.selfDeltaHP    = _res_.selfDeltaHp;        \
    turnInfo.destDeltaHP    = _res_.destDeltaHp;        \
    turnInfo.type           = _type_;                   \
    emit sigTurnInfoReady(turnInfo);                    \
}                                                       \

/**
 * @brief BattleField::BattleField
 * @param userA 用户A指针，从userList里拿
 * @param userB 用户B指针
 * @param pkmA  用PokemonFactory生成的用户A宝可梦
 * @param pkmB  用户B宝可梦
 * @param mode  对战模式
 * @param parent    
 */
BattleField::BattleField(
    User*           userA,
    User*           userB,
    PokemonBase*    pkmA,
    PokemonBase*    pkmB,
    BattleMode      mode,
    QObject*        parent
):  QObject(parent),
    m_mode(mode)
{
    // 初始化对战环境啊嗯
    {
        m_users   = {userA, userB};
        m_pkmList = {pkmA, pkmB};
        m_actions = {"", ""};
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

/**
 * @brief BattleField::~BattleField
 *        析构BattleField
 */
BattleField::~BattleField() {
    delete m_pkmList[0];
    delete m_pkmList[1];
    m_pkmList = {nullptr, nullptr};
    
    qDebug() << "[BATTLE FIELD]: DESTROYED A BATTLE FIELD";
}

/**
 * @brief BattleField::setAction
 *        设置对战回合行为（进行对战操作）
 * @param skillIndex 技能槽index
 * @param userIndex  用户A & B标识
 */
void BattleField::setAction(int skillIndex, int userIndex) {
#ifdef DEBUG_FLAG    
    if (userIndex > 1 || userIndex < 0) {
        return;
    }
    if (skillIndex < 0 || skillIndex > 3) {
        return;
    }
#endif
    if (userIndex == 0) {
        m_actions[0] = getPkmA()->getSkill(skillIndex);
    } else {
        m_actions[1] = getPkmB()->getSkill(skillIndex);
    }
    
    if (m_actions[0].length() != 0 &&
        m_actions[1].length() != 0
    ) {
        turn(m_actions[0], m_actions[1]);
    }
}

/**
 * @brief BattleField::checkBattleIsFinished
 *        检测对战是否结束
 * @return {bool} 若结束则返回true，反之返回false
 */
bool BattleField::checkBattleIsFinished() {
    if (m_pkmList[0]->get_curHP() <= 0) {
        emit sigBattleFinished(m_users[1]);
        return true;
    } else if (m_pkmList[1]->get_curHP() <= 0) {
        emit sigBattleFinished(m_users[0]);
        return true;
    }
    return false;
}

/**
 * @brief BattleField::turn
 *        进行一回合对战
 * @param actionA   用户A操作
 * @param actionB   用户B操作
 */
void BattleField::turn(const QString& actionA, const QString& actionB) {
    PokemonBase& pkmA = *m_pkmList[0];
    PokemonBase& pkmB = *m_pkmList[1];
    
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
            EMIT_TURN_INFO(resA, actionA, A_TO_B);
            {
                pkmA.set_curHP(
                    std::min(pkmA.get_curHP() - resA.selfDeltaHp, pkmA.get_HP()));
                pkmB.set_curHP(
                    std::min(pkmB.get_curHP() - resA.destDeltaHp, pkmB.get_HP()));
            }
            pkmA.printStatus();
            pkmB.printStatus();
            if (checkBattleIsFinished()) {
                return;
            }
            
            auto resB = pkmB.attack(pkmA, actionB);
            handleResult(resB, B_TO_A);
            EMIT_TURN_INFO(resB, actionB, B_TO_A);
            {
                pkmA.set_curHP(
                    std::min(pkmA.get_curHP() - resB.destDeltaHp, pkmA.get_HP()));
                pkmB.set_curHP(
                    std::min(pkmB.get_curHP() - resB.selfDeltaHp, pkmB.get_HP()));
            }
            pkmA.printStatus();
            pkmB.printStatus();
            if (checkBattleIsFinished()) {
                return;
            }
        }
        else if (spdA < spdB) {
            auto resB = pkmB.attack(pkmA, actionB);
            handleResult(resB, B_TO_A);
            EMIT_TURN_INFO(resB, actionB, B_TO_A);
            {
                pkmA.set_curHP(
                    std::min(pkmA.get_curHP() - resB.destDeltaHp, pkmA.get_HP()));
                pkmB.set_curHP(
                    std::min(pkmB.get_curHP() - resB.selfDeltaHp, pkmB.get_HP()));
            }
            pkmA.printStatus();
            pkmB.printStatus();
            if (checkBattleIsFinished()) {                
                return;
            }
                    
            auto resA = pkmA.attack(pkmB, actionA);
            handleResult(resA, A_TO_B);
            EMIT_TURN_INFO(resA, actionA, A_TO_B);
            {
                pkmA.set_curHP(
                    std::min(pkmA.get_curHP() - resA.selfDeltaHp, pkmA.get_HP()));
                pkmB.set_curHP(
                    std::min(pkmB.get_curHP() - resA.destDeltaHp, pkmB.get_HP()));
            }
            pkmA.printStatus();
            pkmB.printStatus();
            if (checkBattleIsFinished()) {                
                return;
            }
        }
    }
    
    m_actions[0] = "";
    m_actions[1] = "";
    
    queryBuffList();
    pkmA.printStatus();
    pkmB.printStatus();
    checkBattleIsFinished();
}



// TODO: 增加LOG
/**
 * @brief BattleField::queryBuffList
 *        回合内处理buffList
 */
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
            Buff& buff = const_cast<Buff&>(buffList.at(i));
            
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
                i--;
            }
        }
    };
    
    // -------- logic code body
    handleBuffList(pkmA, m_buffListA);
    handleBuffList(pkmB, m_buffListB);
    
}

/**
 * @brief BattleField::handleResult
 * @param result    attack返回的AttackResult
 * @param type      A对B || B对A
 */
void BattleField::handleResult(
    AttackResult&   result,
    BalanceType     type
) {
    PokemonBase& pkmA = *m_pkmList[0];
    PokemonBase& pkmB = *m_pkmList[1];
    
    // ------------ lambda for convenience 
    static const auto handleDest = [](
        AttackResult&   result,
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
        
        handleDest(result, pkmA, m_buffListA, pkmB, m_buffListB);
        
    } else if(type == B_TO_A) {
        m_buffListB.push_back(result.selfBuf);
        m_buffListA.push_back(result.destBuf);
        
        handleDest(result, pkmB, m_buffListB, pkmA, m_buffListA);
        
    }
    
}
