#ifndef UNITTEST_H
#define UNITTEST_H

#include <PreCompile.h>
#include "Pokemons/TestPokemonFunction.hpp"
#include <Reflect.hpp>
#include "User/User.h"
#include "BattleField/BattleField.h"

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
    User* userA = new User;
    User* userB = nullptr;
    (void) userA, (void) userB;
}

#endif // UNITTEST_H
