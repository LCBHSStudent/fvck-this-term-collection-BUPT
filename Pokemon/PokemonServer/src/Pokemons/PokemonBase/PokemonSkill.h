#ifndef POKEMONSKILL_H
#define POKEMONSKILL_H

#include <PreCompile.h>
#include <Reflect.hpp>

#define SKILL_FUNC_DEF(_func_) \
    static void _func_(PokemonBase* user, PokemonBase* dest)
#define SKILL_FUNC(_func_) \
    void PokemonSkill::_func_(PokemonBase* user, PokemonBase* dest)

#define REGISTER_METHOD(_method_) \
    static FuncReflectHelper<PokemonSkill, QString, PokemonSkill::SkillFunc>    \
        _##_method_(PokemonSkill::s_skillMap, #_method_, &PokemonSkill::_method_); \
    SKILL_FUNC(_method_) \

class PokemonBase;

class PokemonSkill {
public:    
    using SkillFunc = void(/*PokemonSkill::*/*)(PokemonBase*, PokemonBase*);
public FUNCTION:
    PokemonSkill()  = delete;
    ~PokemonSkill() = default;
    
    static void 
        useSkillByName(
            QString&& name,
            PokemonBase* user = nullptr,
            PokemonBase* dest = nullptr)
        {
            s_skillMap[name](user, dest);
        }
    
    SKILL_FUNC_DEF(FireBall);
    SKILL_FUNC_DEF(GreassLeaf);
    SKILL_FUNC_DEF(WaterBullet);
    SKILL_FUNC_DEF(WindBreath);
    
public RESOURCE:
    static QHash<QString, SkillFunc>
        s_skillMap;
    static std::random_device
        rdev;
    
};

#endif // POKEMONSKILL_H
