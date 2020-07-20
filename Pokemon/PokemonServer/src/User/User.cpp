#include "User.h"
#include "../Pokemons/PokemonBase/PokemonBase.h"
#include "../StorageHelper/StorageHelper.h"

User::User(QString username, QObject* parent):
    QObject(parent),
    m_name(username)
{
    updateUserInfo();
}

void User::updateUserInfo() {
    // ----------UPDATE WIN RATE(?)-------------- //
    StorageHelper::Instance().transaction(
"SELECT TOTAL_BATTLE_TIME, WINNER_TIME \
    FROM user_list WHERE USERNAME=?\
",
    [this](QSqlQuery& query) {
        int totalWinnerTime = query.value(1).toInt(),
            totalBattleTime = query.value(0).toInt();
        this->m_winRate = 
            static_cast<double>(totalWinnerTime)/
            static_cast<double>(totalBattleTime);
    });
    
    StorageHelper::Instance().transaction(
        "SELECT * FROM user_" + m_name,
        [this](QSqlQuery& query) {
            this->m_pokemonList.push_back(query.value(0).toInt());
    });
    
    auto pokemonCnt = m_pokemonList.size();
    if(pokemonCnt >= 15) {
        m_countBadge = BadgeType::GOLDEN;
    } else if(pokemonCnt >= 7) {
        m_countBadge = BadgeType::SILVER;
    } else {
        m_countBadge = BadgeType::BRONZE;
    }
    
    int highLevelCnt = 0;
    StorageHelper::Instance().transaction(
        "SELECT count(*) FROM user_" + m_name + "WHERE PKM_LEVEL=15",
        [&highLevelCnt](QSqlQuery query) {
            highLevelCnt = query.value(0).toInt();
    });
    if(highLevelCnt >= 10) {
        m_qualityBadge = BadgeType::GOLDEN;
    } else if(highLevelCnt >= 5) {
        m_qualityBadge = BadgeType::SILVER;
    } else {
        m_qualityBadge = BadgeType::BRONZE;
    }
    qDebug() << "update user " + m_name + " info finished";
    
}

User::~User() {
    
}

//PokemonBase User::createBattlePkm(uint32 pkmId) {
//    return PokemonBase();
//}
