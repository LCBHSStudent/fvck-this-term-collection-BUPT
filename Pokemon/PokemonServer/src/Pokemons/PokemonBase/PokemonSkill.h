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
    
    SKILL_FUNC_DEF(JJJJ);
    SKILL_FUNC_DEF(FireBall);
    SKILL_FUNC_DEF(GreassLeaf);
    SKILL_FUNC_DEF(WaterBullet);
    SKILL_FUNC_DEF(WindBreath);
    
public RESOURCE:
    static QHash<QString, SkillFunc>
        s_skillMap;
    static std::random_device
        s_rdev;
    static std::default_random_engine
        s_engine;
    static std::uniform_real_distribution<float>
        s_distr;
    
};

#endif // POKEMONSKILL_H
