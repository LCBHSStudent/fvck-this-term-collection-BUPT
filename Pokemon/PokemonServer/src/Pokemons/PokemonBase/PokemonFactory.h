#ifndef POKEMONFACTORY_H
#define POKEMONFACTORY_H

#include <PreCompile.h>
#include "../VariousPokemon.h"
#include "../../StorageHelper/StorageHelper.h"

class PokemonFactory {
public:
    PokemonFactory()  = delete;
    ~PokemonFactory() = default;
public FUNCTION:
    static PokemonBase* CreatePokemon(QString userName, int id) {
        auto& instance = StorageHelper::Instance();
        QString _name;
        QString _skill_1, _skill_2, _skill_3, _skill_4;
        int  _typeId = 0, _level   = 0, _exp    = 0,
             _ATK    = 0, _HP      = 0, _DEF    = 0,
             _SPD    = 0, _attr    = 0, _type   = -1;
        
        instance.transaction(
            "SELECT * FROM `user_" + userName + "` WHERE PKM_ID=?",
            [&](QSqlQuery& query){
                _typeId = query.value(1).toInt();
                _level  = query.value(2).toInt();
                _exp    = query.value(3).toInt();
                _ATK    = query.value(4).toInt();
                _DEF    = query.value(5).toInt();
                _HP     = query.value(6).toInt();
                _SPD    = query.value(7).toInt();
            },
        id);
        instance.transaction(
            "SELECT PKM_NAME, PKM_TYPE, PKM_ATTR,\
             PKM_SKILL_1, PKM_SKILL_2, PKM_SKILL_3,\
             PKM_SKILL_4 FROM `pokemon_info` WHERE PKM_ID=?",
            [&](QSqlQuery query) {
                _name       = query.value(0).toString();
                _type       = query.value(1).toInt();
                _attr       = query.value(2).toInt();
                _skill_1    = query.value(3).toString();
                _skill_2    = query.value(4).toString();
                _skill_3    = query.value(5).toString();
                _skill_4    = query.value(6).toString();
            },
        _typeId);
        
        PokemonBase* newPkm = nullptr;
        
        switch (_type) {
        case PokemonBase::PokemonType::HIGH_ATTACK:
            newPkm = 
                new HighAtkPkm (
                    _name, userName, id, _typeId, _level,
                    _exp,  _ATK, _HP, _DEF, _SPD, _attr
                );
            break;
            
        case PokemonBase::PokemonType::HIGH_HITPOINT:
            newPkm = 
                new HighHpPkm (
                    _name, userName, id, _typeId, _level,
                    _exp,  _ATK, _HP, _DEF, _SPD, _attr
                );
            break;
            
        case PokemonBase::PokemonType::HIGH_DEFENSE:
            newPkm = 
                new HighDefPkm (
                    _name, userName, id, _typeId, _level,
                    _exp,  _ATK, _HP, _DEF, _SPD, _attr
                );
            break;
            
        case PokemonBase::PokemonType::HIGH_SPEED:
            newPkm = 
                new HighSpdPkm (
                    _name, userName, id, _typeId, _level,
                    _exp,  _ATK, _HP, _DEF, _SPD, _attr
                );
            break;
            
        default:
            break;
        }
        
        if (newPkm != nullptr) {
            newPkm->setSkill(0, _skill_1);
            newPkm->setSkill(1, _skill_2);
            newPkm->setSkill(2, _skill_3);
            newPkm->setSkill(3, _skill_4);
        }
        
        return newPkm;
    }
};

#endif // POKEMONFACTORY_H
