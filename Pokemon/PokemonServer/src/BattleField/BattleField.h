#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <PreCompile.h>
#include "../User/User.h"
#include "../Pokemons/PokemonBase/PokemonBase.h"

class BattleField: public QObject {
    Q_OBJECT
public RESOURCE:
    /**
     * @brief The BalanceType enum
     */
    enum BalanceType {
        A_TO_B = 0,
        B_TO_A
    };
    
    /**
     * @brief The BattleMode enum
     */
    enum BattleMode {
        EXP_BATTLE = 0,
        DUEL_BATTLE
    };
    
    /**
     * @brief The TurnInfo struct
     */
    struct TurnInfo {
        BalanceType type;           // A对B 或 B对A
        QString     skillName;      // 使用技能名
        int         selfDeltaHP;    // 自身hp变化量
        int         destDeltaHP;    // 对方hp变化量
        Buff        selfBuff;       // 自身获得的buff
        Buff        destBuff;       // 对方获得的buff
    };
    
public:
    /**
         * @brief BattleField
         * @param userA
         * @param userB
         * @param pkmA
         * @param pkmB
         * @param mode
         * @param parent
         */
        BattleField(
            User*           userA,
            User*           userB,
            PokemonBase*    pkmA,
            PokemonBase*    pkmB,
            BattleMode      mode,
            QObject*        parent = nullptr
        );
    virtual     /*析构对战类*/
        ~BattleField();
    
public FUNCTION:
    void                 /*设定回合行动*/
        setAction(int skillIndex, int userIndex);
    void                 /*进行一回合战斗*/
        turn(const QString& actionA, const QString& actionB);
	User*                /*获取USER A*/
		getUserA() const { return m_users[0]; }
	PokemonBase*        /*获取宝可梦A*/
		getPkmA() const  { return m_pkmList[0]; }
	User*               /*获取User B*/
		getUserB() const { return m_users[1]; }
	PokemonBase*        /*获取宝可梦B*/
		getPkmB() const  { return m_pkmList[1]; }
	BattleMode          /*获取对战类型： 升级 & 决斗*/
        getMode() const  { return m_mode; }
    
private FUNCTION:
    void    /*遍历BuffList 更新属性*/
        queryBuffList();
    bool    /*检测对战是否结束*/
        checkBattleIsFinished();
    void    /*进一步处理attack返回的AttackResult*/
        handleResult(AttackResult& result, BalanceType type);
    
signals:
    void    /*对战结束信号*/
        sigBattleFinished(User* winner);
	void    /*回合信息生成信号*/
		sigTurnInfoReady(TurnInfo info);
    
private RESOURCE:
    BattleMode                  /*对战模式*/
        m_mode;
    std::array<User*, 2>        /*用户指针*/
        m_users = {};
    std::array<PokemonBase*, 2> /*宝可梦指针*/
        m_pkmList = {};
    QList<Buff>                 /*Buff列表A & B*/
        m_buffListA = {},
        m_buffListB = {};
    std::array<QString, 2>      /*用户A & B设定的回合内行动*/
        m_actions = {};
};

#endif // BATTLEFIELD_H
