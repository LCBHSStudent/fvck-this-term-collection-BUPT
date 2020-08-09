#include "User.h"
#include "../Pokemons/PokemonBase/PokemonBase.h"
#include "../StorageHelper/StorageHelper.h"

User::User(QString username, QTcpSocket* socket):
    QObject(nullptr),
    m_name(username),
    m_status(UserStatus::IDLE),
    m_userSocket(socket)
{
    updateUserInfo();
    if (m_pokemonList.size() > 0) {
        m_battlePkmId = m_pokemonList[0];
    }
}

User::User(const User& source) {
    this->m_name            = source.m_name;
    this->m_winRate         = source.m_winRate;
    this->m_countBadge      = source.m_countBadge;
    this->m_qualityBadge    = source.m_qualityBadge;
    this->m_pokemonList     = source.m_pokemonList;
    this->m_userSocket      = source.m_userSocket;
}

void User::updateUserInfo() {
    // ----------UPDATE WIN RATE(?)-------------- //
    StorageHelper::Instance().transaction(
"SELECT TOTAL_BATTLE_TIME, WINNER_TIME \
    FROM `user_list` WHERE USERNAME=?\
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
        "SELECT count(*) FROM `user_" + m_name + "` WHERE PKM_LEVEL=15",
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

void User::battleWon() {
    StorageHelper::Instance().transaction(
        "UPDATE `user_list` SET \
         TOTAL_BATTLE_TIME=TOTAL_BATTLE_TIME+1, WINNER_TIME=WINNER_TIME+1 \
         WHERE USERNAME=?",
         StorageHelper::DEFAULT_FUNC,
         m_name
    );
    updateUserInfo();
}

void User::battleLose() {
    StorageHelper::Instance().transaction(
        "UPDATE `user_list` SET \
         TOTAL_BATTLE_TIME=TOTAL_BATTLE_TIME+1 \
         WHERE USERNAME=?",
         StorageHelper::DEFAULT_FUNC,
         m_name
    );
    updateUserInfo();
}

User::~User() {
    
}

//PokemonBase User::createBattlePkm(uint32 pkmId) {
//    return PokemonBase();
//}
