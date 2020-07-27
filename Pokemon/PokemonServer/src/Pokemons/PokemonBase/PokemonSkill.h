#ifndef POKEMONSKILL_H
#define POKEMONSKILL_H

#include <PreCompile.h>
#include <Reflect.hpp>
#include "PokemonBase.h"

#define SKILL_FUNC_DEF(_func_) \
    static AttackResult _func_(PokemonBase* user, PokemonBase* dest)
#define SKILL_FUNC(_func_) \
    AttackResult PokemonSkill::_func_(PokemonBase* user, PokemonBase* dest)

#define REGISTER_METHOD(_method_) \
    static FuncReflectHelper<PokemonSkill, QString, PokemonSkill::SkillFunc>    \
        _##_method_(PokemonSkill::s_skillMap, #_method_, &PokemonSkill::_method_); \
    SKILL_FUNC(_method_) \

class PokemonBase;

class PokemonSkill {
public:
    using SkillFunc = AttackResult(/*PokemonSkill::*/*)(PokemonBase*, PokemonBase*);
public FUNCTION:
    PokemonSkill()  = delete;
    ~PokemonSkill() = default;
    
    static AttackResult 
        useSkillByName(
            const QString&  name,
            PokemonBase*    user = nullptr,
            PokemonBase*    dest = nullptr
        ) {
            if(!user || !dest) {
                qDebug() << "[PokemonSkill]: 目标或使用者为空";
                return AttackResult();
            }
            if(s_skillMap.count(name)) {
                return s_skillMap[name](user, dest);
            } else {
                qDebug() << "[PokemonSkill]: 不存在技能" << name;
                return AttackResult();
            }
        }
    // 高速移动
    SKILL_FUNC_DEF(Agility);
    SKILL_FUNC_DEF(AppleAcid);
    SKILL_FUNC_DEF(BodySlam);
    SKILL_FUNC_DEF(BulletSeed);
    SKILL_FUNC_DEF(Counter);
    SKILL_FUNC_DEF(DoubleEdge);
    SKILL_FUNC_DEF(DragonPulse);
    SKILL_FUNC_DEF(FireBlast);
    SKILL_FUNC_DEF(FlameCharge);
    SKILL_FUNC_DEF(FlareBlitz);
    SKILL_FUNC_DEF(GrassyTerrain);
    SKILL_FUNC_DEF(IcyWind);
    SKILL_FUNC_DEF(KarateChop);
    SKILL_FUNC_DEF(LeafStorm);
    SKILL_FUNC_DEF(LeechLife);
    SKILL_FUNC_DEF(Liquidation);
    SKILL_FUNC_DEF(MaxSteelspike);
    SKILL_FUNC_DEF(MegaDrain);
    SKILL_FUNC_DEF(MegaPunch);
    SKILL_FUNC_DEF(Outrage);
    SKILL_FUNC_DEF(PoisonSting);
    SKILL_FUNC_DEF(Pound);
    SKILL_FUNC_DEF(Psybeam);
    SKILL_FUNC_DEF(QuickAttack);
    SKILL_FUNC_DEF(QuickGuard);
    SKILL_FUNC_DEF(Reflect);
    SKILL_FUNC_DEF(SandAttack);
    SKILL_FUNC_DEF(SolarBlade);
    SKILL_FUNC_DEF(Supersonic);
    SKILL_FUNC_DEF(SurgingStrikes);
    SKILL_FUNC_DEF(SwordsDance);
    
public RESOURCE:
    static QHash<QString, SkillFunc>
        s_skillMap;
    
};

#endif // POKEMONSKILL_H
