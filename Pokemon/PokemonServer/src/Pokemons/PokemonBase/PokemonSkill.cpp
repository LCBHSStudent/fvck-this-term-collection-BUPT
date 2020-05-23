#include "PokemonSkill.h"

#define REGISTER_SKILL REGISTER_METHOD

QHash<QString, PokemonSkill::SkillFunc> PokemonSkill::s_skillMap = {};

REGISTER_SKILL(FireBall) {
    std::cout << "FireBall!\n";
}

REGISTER_SKILL(GreassLeaf) {
    std::cout << "GreassLeaf!\n";
}

REGISTER_SKILL(WaterBullet) {
    std::cout << "WaterBullet!\n";
}

REGISTER_SKILL(WindBreath) {
    std::cout << "WindBreath!\n";
}
