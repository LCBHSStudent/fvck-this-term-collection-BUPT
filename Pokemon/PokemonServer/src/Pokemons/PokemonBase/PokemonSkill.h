#ifndef POKEMONSKILL_H
#define POKEMONSKILL_H

#include <PreCompile.h>

#define SKILL_FUNC_DEF(_func_) \
    static void _func_(PokemonBase& user, PokemonBase& dest)
#define SKILL_FUNC(_func_) \
    void PokemonSkill::_func_(PokemonBase& user, PokemonBase& dest)

class PokemonSkill {
    class PokemonBase;
    using SkillFunc = void(PokemonSkill::*)(PokemonBase&, PokemonBase&);
public FUNCTION:
    PokemonSkill()  = delete;
    ~PokemonSkill() = default;
    
    SKILL_FUNC_DEF(FireBall) {
        qDebug() << "psn";
    }
    SKILL_FUNC_DEF(GreassLeaf) {
        qDebug() << "pns";
    }
    SKILL_FUNC_DEF(WaterBullet) {
        qDebug() << "nsp";
    }
    SKILL_FUNC_DEF(WindBreath) {
        qDebug() << "nps";
    }
    
public RESOURCE:
    static 
        QHash<QString, SkillFunc>
            s_skillMap;
    static std::random_device rdev;
    
};

#endif // POKEMONSKILL_H
