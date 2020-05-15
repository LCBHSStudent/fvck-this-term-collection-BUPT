#include "Logger.h"

//if(Logger.isRunning()) {
//    Logger.m_bStart = false;
//    Logger.wait();
//}


void Logger::run() {
    
    m_sqlite = QSqlDatabase::addDatabase("QSQLITE");
    m_sqlite.setDatabaseName(m_wstrDataPath + m_DBName + ".db");
    if (!m_sqlite.open()) {
        m_DBName        = "";
        m_DBTableName   = "";
        qDebug() << "Open Database Failed!";
    }
    
    QSqlQuery   sqlQuery;
    QString     strSql = "create table if not exists " + m_DBTableName +
        "(id INTEGER PRIMARY KEY autoincrement,"
         "Timestamp BIGINT,Date text,"
         "Time text,"
         "Level text,"
         "LogInfo text)";
    
    if(!sqlQuery.exec(strSql)) {
        qDebug() << "Error:Failed to create table" << sqlQuery.lastError();
    }
    else {
        qDebug() << "Logger Init Finished!";
    }
    
    while(m_bStart) {
        m_Mutex.lock();
        
        if(!m_listLogInfo.isEmpty()) {
            LogInfo logInfo = m_listLogInfo.takeFirst();
            QString strNum = QString("%1").arg(logInfo.m_nTimeStep);
            QString strSql =
                "INSERT INTO " + m_DBTableName + 
               " VALUES(NULL," + strNum + ", \'" + logInfo.m_strDate + "\',\'" +
                logInfo.m_strTime + "\', \'" + logInfo.m_strLevel + "\', \'" +
                logInfo.m_strLog + "\')";
            
            if(!sqlQuery.exec(strSql)) {
                qDebug()<<sqlQuery.lastError();
            }
            else {
                qDebug()<<"inserted log!";
            }
            m_Mutex.unlock();
        }
        else {
            m_Mutex.unlock();
        }
        QThread::msleep(100);
    }
    
    // close DB
    if(m_sqlite.isValid()) {
        if(m_sqlite.isOpen()) {
            m_sqlite.close();
        }
        m_sqlite = QSqlDatabase::database("", false);
    }
    QString sName = QSqlDatabase::database().connectionName();
    if (QSqlDatabase::contains(sName)) {
        QSqlDatabase::removeDatabase(sName);
    }
}

void Logger::InsertLogInfo(QString strLog, eEVENT_LEVEL elevel) {
    m_Mutex.lock();
    LogInfo logInfo;
    logInfo.m_strLog = strLog;
    QDateTime origin_time =
            QDateTime::fromString("1970-01-01 08:00:00","yyyy-MM-dd hh:mm:ss");
    QDateTime current_time =
            QDateTime::currentDateTime();
    logInfo.m_strDate   = current_time.toString("yyyy-MM-dd");
    logInfo.m_strTime   = current_time.toString("hh:mm:ss.zzz");
    logInfo.m_nTimeStep = origin_time.msecsTo(current_time);
    switch(elevel) {
    case LV_EVENT:
        logInfo.m_strLevel="event";
        break;
    case LV_ERROR:
        logInfo.m_strLevel="error";
        break;
    case LV_WARNING:
        logInfo.m_strLevel="warning";
        break;
    default:
        break;
    }
    m_listLogInfo.push_back(logInfo);
    m_Mutex.unlock();
}

void Logger::InitFilePathAndCreatDatabase(
    QString strDBName,
    QString strDBTableName
) {
    m_DBName = strDBName;
    m_DBTableName = strDBTableName;
    
    QString wstrFilePath = qApp->applicationDirPath();
    QDir    dir;
    
    dir.cd(wstrFilePath);
    if(!dir.exists("Log")) {
        dir.mkdir("Log");
    }
    m_wstrDataPath = wstrFilePath+"/Log/";
}
