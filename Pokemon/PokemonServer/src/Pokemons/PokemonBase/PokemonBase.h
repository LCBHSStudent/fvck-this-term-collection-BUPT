#pragma once

#include <PreCompile.h>
#include "../../StorageHelper/StorageHelper.h"

// 初始化列表宏
#define ARGS_LIST \
    QString             _name,  \
    QString             _user,  \
    int                 _id,    \
    int                 _typeId,\
    int                 _level, \
    int                 _exp,   \
    int                 _ATK,   \
    int                 _HP,    \
    int                 _DEF,   \
    int                 _SPD,   \
    int                 _attr,  \
    int                 _type   \
    
// 定义类用到的初始化列表宏
#define DEF_ARGS_LIST \
    QString             _name,  \
    QString             _user,  \
    int                 _id,    \
    int                 _typeId,\
    int                 _level, \
    int                 _exp,   \
    int                 _ATK,   \
    int                 _HP,    \
    int                 _DEF,   \
    int                 _SPD,   \
    int                 _attr,  \
    int                 _type = UNDEFINED_TYPE \

// 判断宝可梦属性
#define JUDGE_ATTR(_ATTR) \
    (dest->m_pkmAttr == PokemonBase::PokemonAttribute::_ATTR) \

// 判断宝可梦类型
#define JUDGE_PKM_TYPE(_pkm, _type) \
    if(_pkm->m_pkmType == PokemonBase::PokemonType::_type) \

/**
 * @brief The Buff struct
 */
struct Buff {
FUNCTION    
    Buff(): buffId(0), turnCnt(0){}
    Buff(int id, int cnt): buffId(id), turnCnt(cnt){}
    
RESOURCE
    int buffId;     // buff id
    int turnCnt;    // 回合限制
};

/**
 * @brief The PropertyCombo struct
 */
struct PropertyCombo {
    int     hp  = 0;
    int     atk = 0;
    int     def = 0;
    int     spd = 0;
};

/**
 * @brief The AttackResult struct
 */
struct AttackResult {
FUNCTION
    AttackResult() = default;

    AttackResult(
        int _selfId, int _selfCnt,
        int _destId, int _destCnt,
        int _selfDeltaHp,
        int _destDeltaHp
    ):  selfBuf(_selfId, _selfCnt),
        destBuf(_destId, _destCnt),
        selfDeltaHp(_selfDeltaHp),
        destDeltaHp(_destDeltaHp) {}
    
RESOURCE
    Buff selfBuf = {};
    Buff destBuf = {};
    int  selfDeltaHp = 0;
    int  destDeltaHp = 0;
};

/**
 * @brief The PokemonBase class
 */
class PokemonBase {
    friend class PokemonSkill;
public:     // 构造宝可梦类
		 PokemonBase(DEF_ARGS_LIST);
	virtual // 析构宝可梦类
		~PokemonBase() = default;

public RESOURCE:
    /**
     * @brief The PokemonType enum
     */
	enum PokemonType {
		HIGH_ATTACK = 0,
		HIGH_HITPOINT,
		HIGH_DEFENSE,
		HIGH_SPEED,
		UNDEFINED_TYPE
	};
    
    /**
     * @brief The PokemonAttribute enum
     */
    enum PokemonAttribute {
        FIRE = 0,
        WATER,
        GRASS,
        UNDEFINED_ATTR
    };
    
    /**
     * @brief The BuffType enum
     */
    enum BuffType {
        ATK_UP_S = 1,   ATK_UP_M, ATK_UP_L,
        DEF_UP_S,       DEF_UP_M, DEF_UP_L,
        SPD_UP_S,       SPD_UP_M, SPD_UP_L,
        ATK_DOWN_S, ATK_DOWN_M, ATK_DOWN_L,
        DEF_DOWN_S, DEF_DOWN_M, DEF_DOWN_L,
        SPD_DOWN_S, SPD_DOWN_M, SPD_DOWN_L,
        
        REVIVE,
        POISONING,
        DAMAGE_HALF,
        DAMAGE_INVALID,
        SKILL_DAMAGE_UP,
        SKILL_DAMAGE_DOUBLE,
        
        SLIENT,
        SLEEPING,
        PALSYING,
    };
    
	static const int        /*最大等级*/    
        MAX_LEVEL = 15;
    static int              /*升级经验*/
        LEVEL_UP_EXP[MAX_LEVEL-1];
    static PropertyCombo    /*初始属性值（废弃）*/
        INITIAL_PROPERTY;
    
public FUNCTION:
	void   /*经验值变化*/
		gainExperience(int exp);
    
    // ------ property属性组
    PROPERTY(int, id)
    PROPERTY(int, exp)
    PROPERTY(int, level)
    PROPERTY(int, typeID)
    PROPERTY(int, ATK)
    PROPERTY(int, curATK)
    PROPERTY(int, HP)
    PROPERTY(int, curHP)
    PROPERTY(int, DEF)
    PROPERTY(int, curDEF)
    PROPERTY(int, SPD)
    PROPERTY(int, curSPD)
    
    PROPERTY(PokemonType, pkmType)
    PROPERTY(PokemonAttribute, pkmAttr)
    
    PROPERTY(QString, name)
    PROPERTY(QString, curUser)
    PROPERTY(QString, desc)
    
public FUNCTION:
	virtual AttackResult    /*返回AttackResult的攻击函数*/
		attack(
                  PokemonBase&    target,   // 对方宝可梦引用
		    const QString&        skillName // 技能名
		) = 0;
	virtual void            /*升级时调用*/
		levelUp() = 0;
	void                    /*为技能槽设置技能*/
        setSkill(int slot, const QString& name) {
            if (slot < 4 && slot >= 0)    
                m_skills[slot] = name;    
        }
    QString                 /*获取技能槽内技能*/
        getSkill(int slot) const {
            if (slot < 4 && slot >= 0) { 
                return m_skills[slot];
            } else {
                return QString();
            }
        }
    void                    /*打印宝可梦状态信息*/
        printStatus() const;    
    void                    /*存储宝可梦信息到数据库*/
        save2LocalStorage() const;

protected RESOURCE:
	QString     /*宝可梦名称*/
		m_name  = {};
    QString     /*当前所有者用户名*/
        m_curUser = {};
    QString     /*宝可梦描述*/
        m_desc  = {};
    
    int         /*宝可梦持有ID*/
        m_id    = 0;
    int         /*宝可梦等级*/
		m_level = 0;
	int         /*宝可梦当前经验值*/
		m_exp	= 0;
    int         /*宝可梦种族id*/
        m_typeID = 0;

	int         /*宝可梦攻击力 & 当前攻击力*/
		m_ATK  = 0, m_curATK  = 0;
	int         /*宝可梦HP & 当前HP*/
		m_HP   = 0, m_curHP   = 0;
	int         /*宝可梦防御力 & 当前防御力*/
		m_DEF  = 0, m_curDEF  = 0;
	int         /*宝可梦速度 & 当前速度*/
		m_SPD = 0, m_curSPD = 0;
    
	PokemonType         /*宝可梦类型*/
		m_pkmType  = UNDEFINED_TYPE;
    PokemonAttribute    /*宝可梦属性*/
        m_pkmAttr  = UNDEFINED_ATTR;
    QVector<QString>    /*宝可梦技能槽*/
        m_skills   = {};
};
