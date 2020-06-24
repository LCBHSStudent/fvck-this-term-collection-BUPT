#ifndef SERVERBACKEND_H
#define SERVERBACKEND_H

#include <PreCompile.h>
#include "../User/User.h"
#include "../BattleField/BattleField.h"
#include "../NetworkHelper/NetworkHelper.h"

class ServerBackend: public QObject {
    Q_OBJECT
public:
    ServerBackend();
    
public slots:
    void
        slotUserLogin();
    void
        slotUserLogout();
    void
        slotUserSingUp();
    void
        slotRequestUserInfo();
    void 
        slotRequestPokemonInfo();
    
public FUNCTION:
    template<typename... Args>
    [[nodiscard]] QSqlQuery prepare(std::string _query, Args&&... args) {
        QSqlQuery query(m_db);
        query.prepare(_query.data());
        auto a = std::forward_as_tuple(args...);
        std::apply([&query, this](auto&&... args) {
            ((query.addBindValue(QVariant::fromValue(args)), std::cout << args), ...);
        }, a);
        query.exec();
        
        QString&& error = query.lastError().text();
        if(error.isEmpty()) {
            qDebug() << QString(_query.data()) << "succeed!";
        } else {
            qDebug() << error;
        }
        return query;
    }
    
private RESOURCE:
    std::unique_ptr<NetworkHelper>
        m_helper;
    QSqlDatabase
        m_db = {};
    QList<User>
        m_userList = {};
    
    QList<PokemonBase*>
        m_serverPkm = {};
};

#endif // SERVERBACKEND_H
