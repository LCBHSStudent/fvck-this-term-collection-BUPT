#ifndef TESTPOKEMONFUNCTION_H
#define TESTPOKEMONFUNCTION_H

#include "HighAtkPkm.h"
#include "PokemonBase/PokemonSkill.h"

void TestCreatePokemon() {
    using namespace std;
    unique_ptr<HighAtkPkm> testPkm(new HighAtkPkm("test"));
    unique_ptr<HighAtkPkm> testPkm2(new HighAtkPkm("test2"));
    testPkm->attack(*testPkm2);
    
    qDebug() << "fin";
}

void TestPokemonSkillFuncAddr() {
    auto addr1 = (uintptr_t)&PokemonSkill::FireBall;
    auto addr2 = (uintptr_t)&PokemonSkill::GreassLeaf;
    auto addr3 = (uintptr_t)&PokemonSkill::WaterBullet;
    auto addr4 = (uintptr_t)&PokemonSkill::WindBreath;
    qDebug() << addr1 << addr2 << addr3 << addr4;
    qDebug() << addr2 - addr1 << addr2 - addr3 << addr4 - addr3;
}


#endif // TESTPOKEMONFUNCTION_H
