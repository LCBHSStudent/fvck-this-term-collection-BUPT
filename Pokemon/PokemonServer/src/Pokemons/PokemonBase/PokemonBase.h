#pragma once

#include <PreCompile.h>

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

#define JUDGE_ATTR(_ATTR) \
    (dest->m_pkmAttr == PokemonBase::PokemonAttribute::_ATTR) \

#define JUDGE_PKM_TYPE(_pkm, _type) \
    if(_pkm->m_pkmType == PokemonBase::PokemonType::_type) \

struct Buff {
FUNCTION    
    Buff(): buffId(0), turnCnt(0){}
    Buff(int id, int cnt): buffId(id), turnCnt(cnt){}
    
RESOURCE
    int buffId;
    int turnCnt;
};

struct PropertyCombo {
    int     hp  = 0;
    int     atk = 0;
    int     def = 0;
    int     spd = 0;
};

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

class PokemonBase {
    friend class PokemonSkill;
public:
		 PokemonBase(DEF_ARGS_LIST);
	virtual
		~PokemonBase() = default;

public RESOURCE:
	enum PokemonType {
		HIGH_ATTACK = 0,
		HIGH_HITPOINT,
		HIGH_DEFENSE,
		HIGH_SPEED,
		UNDEFINED_TYPE
	};
    enum PokemonAttribute {
        FIRE = 0,
        WATER,
        GRASS,
        UNDEFINED_ATTR
    };
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
    
	static const int    
        MAX_LEVEL = 15;
    static int
        LEVEL_UP_EXP[MAX_LEVEL + 1];
    static PropertyCombo
        INITIAL_PROPERTY;
    
public FUNCTION:
	void
		gainExperience(int exp);
    
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
	virtual AttackResult
		attack(
                  PokemonBase&    target,
		    const QString&        skillName
		) = 0;
	virtual void
		levelUp() = 0;
	void 
        setSkill(int slot, const QString& name) {
            if (slot < 4 && slot >= 0)    
                m_skills[slot] = name;    
        }
    QString
        getSkill(int slot) const {
            if (slot < 4 && slot >= 0) { 
                return m_skills[slot];
            } else {
                return QString();
            }
        }
    void
        printStatus() const ;

protected RESOURCE:
	QString
		m_name  = {};
    QString
        m_curUser = {};
    QString
        m_desc  = {};
    
    int   
        m_id    = 0;
    int   
		m_level = 0;
	int   
		m_exp	= 0;
    int
        m_typeID = 0;

	int   
		m_ATK  = 0, m_curATK  = 0;
	int   
		m_HP   = 0, m_curHP   = 0;
	int   
		m_DEF  = 0, m_curDEF  = 0;
	int   
		m_SPD = 0, m_curSPD = 0;
    
	PokemonType
		m_pkmType  = UNDEFINED_TYPE;
    PokemonAttribute
        m_pkmAttr  = UNDEFINED_ATTR;
    QVector<QString>
        m_skills   = {};
};
