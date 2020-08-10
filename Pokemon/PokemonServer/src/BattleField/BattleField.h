#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <PreCompile.h>
#include "../User/User.h"
#include "../Pokemons/PokemonBase/PokemonBase.h"

class BattleField: public QObject {
    Q_OBJECT
public RESOURCE:
    enum BalanceType {
        A_TO_B = 0,
        B_TO_A
    };
    enum BattleMode {
        EXP_BATTLE = 0,
        DUEL_BATTLE
    };

    struct TurnInfo {
        BalanceType type;
        QString     skillName;
        int         selfDeltaHP;
        int         destDeltaHP;
        Buff        selfBuff;
        Buff        destBuff;
    };
    
public:
        BattleField(
            User*           userA,
            User*           userB,
            PokemonBase*    pkmA,
            PokemonBase*    pkmB,
            BattleMode      mode,
            QObject*        parent = nullptr
        );
    virtual 
        ~BattleField();
    
public FUNCTION:
    void
        setAction(int skillIndex, int userIndex);
    void 
        turn(const QString& actionA, const QString& actionB);
	User*
		getUserA() const { return m_users[0]; }
	PokemonBase*
		getPkmA() const  { return m_pkmList[0]; }
	User*
		getUserB() const { return m_users[1]; }
	PokemonBase*
		getPkmB() const  { return m_pkmList[1]; }
	BattleMode
        getMode() const  { return m_mode; }
    
private FUNCTION:
    void
        queryBuffList();
    bool
        checkBattleIsFinished();
    void
        handleResult(AttackResult& result, BalanceType type);
    
signals:
    void
        sigBattleFinished(User* winner);
	void
		sigTurnInfoReady(TurnInfo info);
    
private RESOURCE:
    BattleMode
        m_mode;
    std::array<User*, 2>
        m_users = {};
    std::array<PokemonBase*, 2>
        m_pkmList = {};
    QList<Buff>
        m_buffListA = {},
        m_buffListB = {};
    std::array<QString, 2>
        m_actions = {};
};

#endif // BATTLEFIELD_H
