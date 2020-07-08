#ifndef STORAGEHELPER_H
#define STORAGEHELPER_H

#include <PreCompile.h>

class StorageHelper: public QObject {
    Q_OBJECT
public FUNCTION:
    static const StorageHelper& Instance() {
        if(!m_instance) {
            QMutexLocker locker(&m_instMtx);
            if(!m_instance) {
                m_instance = new StorageHelper;
            }
        }
        return *m_instance;
    }
    // consider to use exception?
    template<typename... Args>
    void transaction(
        const QString& queryStat,
        std::function<void(QSqlQuery&)> process,
        Args&&... args
    ) const {
        QSqlQuery query(m_db);
        query.prepare(queryStat);
        
        auto argTuple = std::forward_as_tuple(args...);
        std::apply([&query](auto&&... args) {
            ((query.addBindValue(QVariant::fromValue(args))), ...);
        }, argTuple);
        
        query.exec();
//        qDebug() << (
//            query.lastError().text().isEmpty()?
//            _query + "succeed":
//            query.lastError().text()
//        );
        if(query.lastError().text().isEmpty()) {
            qDebug() << "[SQL TRANSACTION] " + queryStat + " succeed";
            while(query.next()) {
                process(query); // query.value(index).toAnyDataFormat
            }
            return;
        } else {
            qDebug() << query.lastError().text();
            return;
        }
    }
    
private FUNCTION:
    StorageHelper();
    ~StorageHelper() = default;
    
private RESOURCE:
    static QMutex 
        m_instMtx;
    static StorageHelper*
        m_instance;
    QSqlDatabase
        m_db = {};
};

#endif // STORAGEHELPER_H
