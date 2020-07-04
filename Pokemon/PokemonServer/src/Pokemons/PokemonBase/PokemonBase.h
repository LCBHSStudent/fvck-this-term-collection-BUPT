#pragma once

#include <PreCompile.h>

#define ARGS_LIST \
    QString             _name,  \
    int                 _id,    \
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
    int                 _id,    \
    int                 _level, \
    int                 _exp,   \
    int                 _ATK,   \
    int                 _HP,    \
    int                 _DEF,   \
    int                 _SPD,   \
    int                 _attr,  \
    int                 _type = UNDEFINED_TYPE \

struct Buff {
FUNCTION    
    Buff(): buffId(0), turnCnt(0){}
    Buff(int id, int cnt): buffId(id), turnCnt(cnt){}
    
RESOURCE
    int buffId;
    int turnCnt;
};

struct BuffResult {
FUNCTION
    BuffResult(): selfBuf(), destBuf(){}
    BuffResult(
        int _selfId, int _selfCnt,
        int _destId, int _destCnt
    ):  selfBuf(_selfId, _selfCnt),
        destBuf(_destId, _destCnt) {}
    
RESOURCE
    Buff selfBuf;
    Buff destBuf;
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
        ATK_UP_S = 0,   ATK_UP_L, ATK_UP_M,
        DEF_UP_S,       DEF_UP_L, DEF_UP_M,
        SPD_UP_S,       SPD_UP_L, SPD_UP_M,
        ATK_DOWN_S, ATK_DOWN_L, ATK_DOWN_M,
        DEF_DOWN_S, DEF_DOWN_L, DEF_DOWN_M,
        SPD_DOWN_S, SPD_DOWN_L, SPD_DOWN_M,
        
        REVIVE,
        POISONING,
        DAMAGE_HALF,
        DAMAGE_INVALID,
        
        SLIENT,
        SLEEPING,
        PALSYING,
    };
    
	static const int    
        MAX_LEVEL = 15;
    static int
        LEVEL_UP_EXP[15];
    
public FUNCTION:
	void
		gainExperience(int exp);
    
    PROPERTY(int, id)
    PROPERTY(int, exp)
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
    
public FUNCTION:
	virtual BuffResult
		attack(
                  PokemonBase&    target,
		    const QString&        skillName
		) = 0;
	virtual void
		levelUp() = 0;
	void 
        setSkill(int    slot, const QString& name) {
            m_skills[slot] = name;    
        }

protected RESOURCE:
	QString
		m_name  = {};
    int   
        m_id    = 0;
    int   
		m_level = 0;
	int   
		m_exp	= 0;

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
    std::array<QString, 4>
        m_skills   = {};
};
