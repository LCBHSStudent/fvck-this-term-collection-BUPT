#ifndef UNITTEST_H
#define UNITTEST_H

#include <PreCompile.h>
#include "Pokemons/TestPokemonFunction.hpp"
#include <Reflect.hpp>
#include "User/User.h"
#include "BattleField/BattleField.h"
#include "NetworkHelper/NetworkHelper.h"

void TestReflect() {
    PokemonSkill::useSkillByName("FireBall");
}

class Test {
public:
    Test() = default;
    explicit Test(int a):m_a(a) {}
    
    friend const std::ostream& operator<< (
        std::ostream &os,
        const Test &t
    ) {
        (void)t.m_a;
        return os << "test";
    }
    
private:
    int m_a = 0;
};

Q_DECLARE_METATYPE(Test)

void TestCompileArglistQuery() {
    auto  f =3;
    QVariant d;
    qDebug() << d.fromValue(f);
    qDebug() << d;
}

void TestUserBattle() {
//    User* userA = new User;
//    User* userB = nullptr;
//    (void) userA, (void) userB;
//    HighAtkPkm hatk("攻击", 1, 1, 0, 1, 100, 0, 0, PokemonBase::FIRE);
//    hatk.setSkill(0, "JJJJ");
//    HighDefPkm hdef("守备", 1, 1, 0, 0, 100, 0, 0, PokemonBase::WATER);
//    hdef.setSkill(0, "JJJJ");
//    BattleField bf = BattleField(userA, userB, &hatk, &hdef);
//    bf.turn("JJJJ", "JJJJ");
}

void TestMarshalProto() {
    
}

#endif // UNITTEST_H
