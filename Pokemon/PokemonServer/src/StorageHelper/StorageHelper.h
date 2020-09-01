#ifndef STORAGEHELPER_H
#define STORAGEHELPER_H

#include <PreCompile.h>

class StorageHelper: public QObject {
    Q_OBJECT
public FUNCTION:
    /**
     * @brief Instance 单例模式
     * @return 实例引用
     */
    static const StorageHelper& Instance() {
        if(!m_instance) {
            QMutexLocker locker(&m_instMtx);
            if(!m_instance) {
                m_instance = new StorageHelper;
            }
        }
        return *m_instance;
    }
    /**
     * @brief DEFAULT_FUNC
     *        默认的Query处理函数
     */
    static inline auto DEFAULT_FUNC = std::function<void(QSqlQuery&)>();
    
    // consider to use exception?
    
    // 绑定可变参数包，采用回调获取数据的transaction函数
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
#ifdef DEBUG_FLAG
            qDebug() << "[SQL TRANSACTION] " + queryStat + " succeed";
#endif
            while(query.next()) {
                process(query); // query.value(index).toAnyDataFormat
            }
            return;
        } else {
            qDebug() << "[SQL TRANSACTION ERROR] " << query.lastError().text();
            return;
        }
    }
    
private FUNCTION:
    StorageHelper();            // 构造函数私有化
    ~StorageHelper() = default; // 默认析构函数
    
private RESOURCE:
    static QMutex           // 单例锁
        m_instMtx;
    static StorageHelper*   // 单例指针
        m_instance;
    QSqlDatabase            // 数据库连接
        m_db = {};
};

#endif // STORAGEHELPER_H
