#include "PokemonSkill.h"
#include "PokemonBase.h"

#define REGISTER_SKILL REGISTER_METHOD

// ----------DEFINATIONS FOR CONVIENCE---------- //

#define ATK_DEBUG(_SKILL) \
    qDebug()  << user->m_name \
              + "使用" #_SKILL "攻击了" \
              + dest->m_name + "\n" \

// ---------GLOBAL VARIABLES---------- //
QHash<QString, PokemonSkill::SkillFunc> 
    PokemonSkill::s_skillMap = {};
// ----------------------------------- //


// ------伤害公式：[(1 * 等级 + 10) / 250 * (ATK / DEF) * SkillPower + 1] * 倍率
#define DAMAGE(_POWER) \
    calDamage(\
        static_cast<double>(user->m_level),\
        static_cast<double>(user->m_curATK),\
        static_cast<double>(dest->m_curDEF),\
        static_cast<double>(_POWER)\
    )
constexpr auto calDamage = [](
    double  userLevel,
    double  userATK,
    double  destDEF,
    double  skillPower
) {
    double part1 = (1.0f * userLevel + 10) / 250;
    double part2 = userATK / destDEF;
    return static_cast<int>(part1 * part2 * skillPower + 1);
};


// ------------DEFINATION OF SKILLS------------ //
REGISTER_SKILL(Agility) {
    ATK_DEBUG(高速移动);
    AttackResult result;
    {
        result.selfBuf.buffId   = PokemonBase::BuffType::SPD_UP_L;
        result.selfBuf.turnCnt  = 3;
    }
    return result;
}

REGISTER_SKILL(AppleAcid) {
    ATK_DEBUG(苹果酸);
    AttackResult result;
    JUDGE_OZ(80) {
        result.destBuf.buffId  = PokemonBase::BuffType::DEF_DOWN_M;
        result.destBuf.turnCnt = 2;
    }
    
    result.destDeltaHp = DAMAGE(80);
    if JUDGE_ATTR(FIRE) {
        result.destDeltaHp *= 1.2;
    } else if JUDGE_ATTR(GRASS) {
        result.destDeltaHp *= 0.8;
    }
    
    return result;
}

REGISTER_SKILL(BodySlam) {
    ATK_DEBUG(泰山压顶);
    AttackResult result;
    JUDGE_OZ(20) {
        result.destBuf.buffId  = PokemonBase::BuffType::PALSYING;
        result.destBuf.turnCnt = 2;
    }
    result.destDeltaHp = DAMAGE(85);
    
    return result;
}

REGISTER_SKILL(BulletSeed) {
    ATK_DEBUG(种子机关枪);
    AttackResult result;
    
    int repeat = 2 + s_distr(s_engine) * 3;
    for (int i = 0; i < repeat; i++) {
        result.destDeltaHp += DAMAGE(25);
    }
    
    return result;
}

REGISTER_SKILL(Counter) {
    ATK_DEBUG(绝地反击);
    AttackResult result;
    
    result.destDeltaHp = DAMAGE(65);
    double rate = 1.0f + 
        static_cast<double>(user->m_HP - user->m_curHP) / 
        static_cast<double>(user->m_HP);
    result.destDeltaHp *= rate;
    
    return result;
}

REGISTER_SKILL(DoubleEdge) {
    ATK_DEBUG(舍身撞击);
    AttackResult result;
    
    result.destDeltaHp = DAMAGE(120);
    result.selfDeltaHp = result.destDeltaHp * 0.4;
    
    return result;
}

REGISTER_SKILL(DragonPulse) {
    ATK_DEBUG(龙之波动);
    AttackResult result;
    
    result.destDeltaHp = DAMAGE(85);
    
    return result;
}

REGISTER_SKILL(FireBlast) {
    ATK_DEBUG(大字爆炎);
    AttackResult result;
    
    JUDGE_OZ(30) {
        result.destBuf.buffId  = PokemonBase::BuffType::POISONING;
        result.destBuf.turnCnt = 3;
    }
    result.destDeltaHp = DAMAGE(110);
    
    return result;
}

REGISTER_SKILL(FlameCharge) {
    ATK_DEBUG(蓄能焰袭);
    AttackResult result;
    
    result.destBuf.buffId   = PokemonBase::BuffType::SPD_UP_L;
    result.destBuf.turnCnt  = 2;
    result.destDeltaHp    = DAMAGE(50);
    
    return result;
}

REGISTER_SKILL(FlareBlitz) {
    ATK_DEBUG(闪焰冲锋);
    AttackResult result;
    
    JUDGE_OZ(30) {
        result.destBuf.buffId  = PokemonBase::BuffType::POISONING;
        result.destBuf.turnCnt = 3;
    }
    
    result.destDeltaHp = DAMAGE(110);
    result.selfDeltaHp = result.destDeltaHp * 0.45;
    
    return result;
}

REGISTER_SKILL(GrassyTerrain) {
    ATK_DEBUG(青草场地);
    AttackResult result;
    
    result.selfBuf.buffId   = PokemonBase::BuffType::SKILL_DAMAGE_UP;
    result.selfBuf.turnCnt  = 0x3F;
    result.selfDeltaHp      = -(user->m_HP * 0.2);
    result.destBuf.buffId   = PokemonBase::BuffType::ATK_DOWN_S;
    result.destBuf.turnCnt  = 1;
    
    return result;
}

REGISTER_SKILL(IcyWind) {
    ATK_DEBUG(冰冻之风);
    AttackResult result;
    
    result.destBuf.buffId   = PokemonBase::BuffType::SPD_DOWN_M;
    result.destBuf.turnCnt  = 3;
    result.destDeltaHp      = DAMAGE(75);
    
    return result;
}

REGISTER_SKILL(KarateChop) {
    ATK_DEBUG(空手劈);
    AttackResult result;
    
    result.destDeltaHp = DAMAGE(75);
    
    return result;
}

REGISTER_SKILL(LeafStorm) {
    ATK_DEBUG(飞叶风暴);
    AttackResult result;
    
    result.selfBuf.buffId   = PokemonBase::BuffType::ATK_DOWN_L;
    result.selfBuf.turnCnt  = 2;
    result.destDeltaHp      = DAMAGE(130);
    
    if JUDGE_ATTR(WATER) {
        result.destDeltaHp *= 1.2;
    }
    
    return result;
}

REGISTER_SKILL(LeechLife) {
    ATK_DEBUG(吸血);
    AttackResult result;
    
    result.destDeltaHp = DAMAGE(75);
    result.selfDeltaHp = -(result.destDeltaHp * 0.5);
    
    return result;
}

REGISTER_SKILL(Liquidation) {
    ATK_DEBUG(水流裂破);
    AttackResult result;
    
    result.destDeltaHp = DAMAGE(90);
    JUDGE_OZ(50) {
        result.destBuf.buffId   = PokemonBase::BuffType::DEF_DOWN_M;
        result.destBuf.turnCnt  = 3;
    }
    
    if JUDGE_ATTR(FIRE) {
        result.destDeltaHp *= 1.3;
    }
    
    return result;
}

REGISTER_SKILL(MaxSteelspike) {
    ATK_DEBUG(极巨钢铁);
    AttackResult result;
    
    result.destDeltaHp      = DAMAGE(100);
    result.selfBuf.buffId   = PokemonBase::DEF_UP_L;
    result.selfBuf.turnCnt  = 2;
    
    return result;
}

REGISTER_SKILL(MegaDrain) {
    ATK_DEBUG(超级吸取);
    AttackResult result;
    
    result.destDeltaHp = DAMAGE(85);
    result.selfDeltaHp = -(result.destDeltaHp * 0.5);
    
    return result;
}

REGISTER_SKILL(MegaPunch) {
    ATK_DEBUG(百万吨重拳);
    AttackResult result;
    
    result.destDeltaHp = DAMAGE(110);
    
    return result;
}

REGISTER_SKILL(Outrage) {
    ATK_DEBUG(逆鳞);
    AttackResult result;
    
    result.selfBuf.buffId   = PokemonBase::BuffType::SLEEPING;
    result.selfBuf.turnCnt  = 2;
    
    result.destDeltaHp = DAMAGE(75);
    JUDGE_OZ(70) {
        result.destDeltaHp += DAMAGE(65);
        JUDGE_OZ(60) {
            result.destDeltaHp += DAMAGE(50);
        }
    }
    
    return result;
}

REGISTER_SKILL(PoisonSting) {
    ATK_DEBUG(毒针);
    AttackResult result;
    
    result.destBuf.buffId   = PokemonBase::BuffType::POISONING;
    result.destBuf.turnCnt  = 5;
    
    result.destDeltaHp = DAMAGE(50);
    
    return result;
}

REGISTER_SKILL(Pound) {
    ATK_DEBUG(拍击);
    AttackResult result;
    
    result.destDeltaHp = DAMAGE(70);
    
    return result;
}

REGISTER_SKILL(Psybeam) {
    ATK_DEBUG(幻象光线);
    AttackResult result;
    
    JUDGE_OZ(70) {
        result.destBuf.buffId   = PokemonBase::BuffType::PALSYING;
        result.destBuf.turnCnt  = 3;
    }
    
    result.destDeltaHp = DAMAGE(40);
    
    return result;
}

REGISTER_SKILL(QuickAttack) {
    ATK_DEBUG(电光一闪);
    AttackResult result;
    
    result.destDeltaHp = DAMAGE(70);
    result.selfBuf.buffId   = PokemonBase::BuffType::SPD_UP_L;
    result.selfBuf.turnCnt  = 2;
    
    return result;
}

REGISTER_SKILL(QuickGuard) {
    ATK_DEBUG(迂回);
    AttackResult result;
    
    if(dest->m_curSPD > user->m_curSPD) {
        result.selfBuf.buffId   = PokemonBase::BuffType::DAMAGE_INVALID;
        result.selfBuf.turnCnt  = 2;
    }
    
    return result;
}

REGISTER_SKILL(Reflect) {
    ATK_DEBUG(反射壁);
    AttackResult result;
    
    result.selfBuf.buffId   = PokemonBase::BuffType::DAMAGE_HALF;
    result.selfBuf.turnCnt  = 3;
    
    return result;
}

REGISTER_SKILL(SandAttack) {
    ATK_DEBUG(泼沙);
    AttackResult result;
    
    result.destDeltaHp      = DAMAGE(60);
    result.destBuf.buffId   = PokemonBase::BuffType::SPD_DOWN_M;
    result.destBuf.turnCnt  = 3;
    
    return result;
}

REGISTER_SKILL(SolarBlade) {
    ATK_DEBUG(日光刃);
    AttackResult result;
    
    result.selfBuf.buffId   = PokemonBase::BuffType::SKILL_DAMAGE_DOUBLE;
    result.selfBuf.turnCnt  = 2;
    
    return result;
}

REGISTER_SKILL(Supersonic) {
    ATK_DEBUG(超音波);
    AttackResult result;
    
    JUDGE_OZ(40) {
        result.destBuf.buffId   = PokemonBase::SLEEPING;
        result.destBuf.turnCnt  = 2;
    }
    
    result.destDeltaHp = DAMAGE(40);
    
    return result;
}

REGISTER_SKILL(SurgingStrikes) {
    ATK_DEBUG(水流连打);
    AttackResult result;
    
    result.destDeltaHp = DAMAGE(75) * 2;
    
    return result;
}

REGISTER_SKILL(SwordsDance) {
    ATK_DEBUG(剑舞);
    AttackResult result;
    
    result.selfBuf.buffId   = PokemonBase::BuffType::ATK_UP_L;
    result.selfBuf.turnCnt  = 3;
    
    return result;
}

/*
REGISTER_SKILL() {
    ATK_DEBUG();
    AttackResult result;
    
    return result;
}
*/
// ------------------------------------- //
