#include "BackendBase.h"

#include <QtConcurrent/QtConcurrent>
#include <iostream>
#include <thread>

class AlgorithmHelper;

BackendBase::BackendBase(QQuickItem *parent):
    QQuickItem      (parent),
    m_sysTime       (QDateTime::currentDateTime()),
    m_cityData      (new CityModel),
    m_scheduleData  (new ScheduleModel),
    m_absData       (new AbstractData),
    m_customer      (new Customer(0))
{
    // copy sqlite file
    {
        QFile preDB("./EmulatorConfig.db");
        if (preDB.exists()) {
            QFile::setPermissions(
                "./EmulatorConfig.db",
                QFile::ReadOther | QFile::WriteOther
            );
            
            if(!preDB.remove()) {
                qDebug() << preDB.errorString();
            }
        }
        QFile dbFile(":/res/data/EmulatorConfig.db");
        dbFile.copy("./EmulatorConfig.db");
//        if(!QFileInfo("./EmulatorConfig.db").exists()) {
//            QFile dbFile(":/res/data/EmulatorConfig.db");
//            dbFile.copy("./EmulatorConfig.db");
//        }
        
    }
    m_sqlite = QSqlDatabase::contains("qt_sql_default_connection")?
               QSqlDatabase::database("qt_sql_default_connection"):
               QSqlDatabase::addDatabase("QSQLITE", "CONFIG");
    m_sqlite.setDatabaseName("./EmulatorConfig.db");
    slotLoadData();
    
    emit sysTimeChanged();
//  connect(this, &BackendBase::sigLoadDataFinished, this, [this]{mainLoop();});
    connect(
        m_customer, &Customer::posChanged,
        this,       &BackendBase::slotCustomerPosChanged);
}


BackendBase::~BackendBase() {
    if(m_sqlite.isOpen()) {
        qDebug() << "closing database";
        m_sqlite.close();
    }
    if(m_cityData) {
        delete m_cityData;
        m_cityData = nullptr;
    }
    if(m_scheduleData) {
        delete m_scheduleData;
        m_scheduleData = nullptr;
    } 
    if(m_absData) {
        delete m_absData;
        m_absData = nullptr;
    }
    if(m_customer) {
        delete m_customer;
        m_customer = nullptr;
    }
    m_running = m_mlPaused = false;
}

void BackendBase::slotLoadData() {
    QtConcurrent::run(this, &BackendBase::loadData);
}

void BackendBase::loadData() {
    if(!m_sqlite.isOpen()) {
        qDebug() << "opening database";
        if(!m_sqlite.open()) {
            qDebug() << "failed to open database\n\t"
                     << m_sqlite.lastError();
            emit sigCloseRootWindow();
        }
    }
    
    QSqlQuery query(m_sqlite);
    if (query.exec("SELECT * FROM City")) {
        size_t count = 0;
        while (query.next()) {
            count++;
            QString cityName = query.value(1).toString();
            AbstractCity city(
                cityName,
                query.value(0).toInt(),
                query.value(2).toFloat()
            );
            m_cityData->append(
                city.id,
                city.name,
                city.factor
            );
            m_absData->m_cityHash[city.name] = city;
            m_absData->m_cityIdHash[city.id] = city;
        }
        m_absData->m_routeMap.resize(count);
    }
    if (query.exec("SELECT * FROM Schedule")) {
        while (query.next()) {
            int       TACNumber     = query.value(0).toInt();
            QString   fromStation   = query.value(1).toString();
            QString   toStation     = query.value(2).toString();
            int       startH        = query.value(3).toInt();
            int       startM        = query.value(4).toInt();
            int       arriveH       = query.value(5).toInt();
            int       arriveM       = query.value(6).toInt();
            int       type          = query.value(7).toInt();
            
            m_scheduleData->append(
                TACNumber,
                fromStation,
                toStation,
                startH,
                startM,
                arriveH,
                arriveM,
                type       
            );
            
            m_absData->m_routeMap[
                m_absData->m_cityHash[fromStation].id
            ].emplace_back(
                AbstractRoute(
                    type,
                    m_absData->m_cityHash[toStation].id,
                    startH * 60 + startM,
                    arriveH* 60 + arriveM,
                    TACNumber
                )
            );
        }
    }
    
    emit cityModelChanged();
    emit scheduleModelChanged();
    
    emit sigLoadDataFinished();
    
    mainLoop();
}

void BackendBase::mainLoop() {
    uint8_t ten = 0;
    while(m_running) {
        while(m_mlPaused && m_running) 
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if(ten == 10) {
            m_sysTime =  m_sysTime.addSecs(360);
            emit sysTimeChanged();
            ten = 0;
        }
        QString log;
        m_customer->updateStatus(log);
        if(log.length()) {
            emit sigNewMessage(m_sysTime.toString("[yyyy-MM-dd HH:mm] ") + log);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ten++;
    }
}

void BackendBase::slotStartQuery(
    QString     fromCity,
    QString     destCity,
    int         startH,
    int         startM,
    int         policy,
    int         timeLmt
) {
    if(fromCity == destCity) 
        return;
    if(startH >= 24 || startH < 0)
        return;
    if(startM >= 60 || startM < 0)
        return;
    
//    m_absData->printAllRouteInfo(fromCity);
    
    emit sigNewMessage(m_sysTime.toString("[yyyy-MM-dd HH:mm]") + "Start query");
    m_customer->setSysTime(
        m_sysTime.time().hour()*3600 + m_sysTime.time().minute()*60
    );
    switch (policy) {
    case TimeRefered:
        AlgorithmHelper::runWithAlgorithmDij(
            this, fromCity, destCity, startH*60 + startM, timeLmt);
        break;
        
    case SafetyRefered:
        AlgorithmHelper::runWithAlgorithmDij(
            this, fromCity, destCity, startH*60 + startM);
        
        break;
    default:
        emit sigNewMessage("\n\tUnkonw policy");
        break;
    }
}

void BackendBase::slotCustomerPosChanged(
    QString from,
    QString dest,
    qint64  duration
) {
    auto& hash = m_absData->m_cityHash;
    
    emit sigCustomerPosChanged(
        hash[from].id,
        hash[dest].id,
        duration / 6 * 1000
    );
    
    qDebug() << from << dest << duration;
}


void BackendBase::invokePauseMainThrd() {
    m_mlPaused = true;
    //m_mainLoopMtx.lock();
}

void BackendBase::invokeContinueMainThrd() {
    m_mlPaused = false;
    //m_mainLoopMtx.unlock();
}
