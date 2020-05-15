#ifndef LOGGER_H
#define LOGGER_H

#include <QThread>
#include <QList>
#include <QTime>
#include <QMutex>
#include <QtCore/QCoreApplication>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

enum eEVENT_LEVEL {
    LV_INVALID = -1,
    LV_EVENT = 0,
    LV_WARNING,
    LV_ERROR
};

struct LogInfo {
    qint64      m_nTimeStep;
    QString     m_strDate;
    QString     m_strTime;
    QString     m_strLog;
    QString     m_strLevel;
};

class Logger: public QThread {
    Q_OBJECT
public:
    Logger(): m_bStart(false) {
        InitFilePathAndCreatDatabase("TravelSystemLog", "Log");
        m_bStart = true;
        start();
    }
    ~Logger() = default;
    
    void 
        InsertLogInfo(
            QString strLog,
            eEVENT_LEVEL elevel = eEVENT_LEVEL::LV_INVALID
        );
    
public:
    bool     
        m_bStart;
    QString  
        m_wstrDataPath;
    QString  
        m_DBName;
    QString  
        m_DBTableName;
    
private:
    void 
        run();
    void 
        InitFilePathAndCreatDatabase(
            QString strDBName, 
            QString strDBTableName
        );
    
    QList<LogInfo>
        m_listLogInfo;
    QMutex          
        m_Mutex;
    QSqlDatabase
        m_sqlite;
};

#endif // LOGGER_H
