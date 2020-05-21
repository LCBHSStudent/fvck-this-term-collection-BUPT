#include "HighAtkPkm.h"

HighAtkPkm::HighAtkPkm(const std::string_view name) {
	m_name = name;
}

uint32 HighAtkPkm::attack(
        PokemonBase&    target,
        uint32          skillIndex
) {
    switch (skillIndex) {
    case 0:
        qDebug() << "goto test brench";
        return 0;
    default:
        qDebug() << "using an unknown skill";
        return 0;
    }
}

void HighAtkPkm::levelUp() {
    
}
