#ifndef BACKENDBASE_H
#define BACKENDBASE_H

#include <QDebug>
#include <QMutex>
#include <QObject>
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QQuickItem>
#include <QSqlDatabase>

#include <chrono>
#include <ctime>

// 鉴于使用模板类时要使用Variant数组作开销，固直接实例化两个Model
#include "DataModel/CityModel.h"
#include "DataModel/ScheduleModel.h"
#include "DataModel/AbstractData.h"
#include "Customer/Customer.h"

#define FUNCTION 
#define RESOURCE 
#define Q_RESOURCE 

class BackendBase : public QQuickItem {
    Q_OBJECT
    
    friend class AlgorithmHelper;
    
    Q_PROPERTY(
        CityModel*  cityModel
        READ        getCityModel
        NOTIFY      cityModelChanged)
    Q_PROPERTY(
        QString     sysTime
        READ        getSysTime
        NOTIFY      sysTimeChanged)
    Q_PROPERTY(
        ScheduleModel*  scheduleModel
        READ            getScheduleModel
        NOTIFY          scheduleModelChanged)
    
public:
    explicit
        BackendBase(QQuickItem *parent = nullptr);
    virtual
        ~BackendBase();
    
public RESOURCE:
    Q_ENUMS(TransType)
    Q_ENUMS(TravelPolicy)
    enum TransType {
        Airplane = 0,
        Train,
        Car
    };
    enum TravelPolicy {
        SafetyRefered,
        TimeRefered
    };
    
public slots:
    void
        slotLoadData();
    void
        invokePauseMainThrd();
    void
        invokeContinueMainThrd();
    void
        slotStartQuery(
            QString     fromCity,
            QString     destCity,
            int         startH,
            int         startM,
            int         policy,
            int         timeLmt = 0x3F3F3F3F
        );
    
    CityModel* 
        getCityModel() const {
            return m_cityData;
        }
    ScheduleModel* 
        getScheduleModel() const {
            return m_scheduleData;
        }
    
// reserve for time info
    QString   
        getSysTime() const {
            return m_sysTime.toString("yyyy-MM-dd HH:mm");
        }
    
    void
        setRunning(bool flag) {m_running = flag;}
    void
        slotCustomerPosChanged(
            QString from,
            QString dest,
            qint64  duration
        );
    
signals:
    void 
        sigLoadDataFinished();
    void
        cityModelChanged();
    void
        scheduleModelChanged(); 
    // reserve for time info
    void 
        sysTimeChanged();
    
    void 
        sigCloseRootWindow();
    void 
        sigCustomerPosChanged(
            qint64 fromId,
            qint64 destId,
            qint64 duration
        );
    void
        sigNewMessage(QString msg);
    
    
private FUNCTION:
    void
        loadData();
    void 
        mainLoop();
    
private Q_RESOURCE:
    QSqlDatabase
        m_sqlite;
    QDateTime
        m_sysTime {};
//    QMutex
//        m_mainLoopMtx {};
    
private RESOURCE:
    CityModel*
        m_cityData      = nullptr;
    ScheduleModel*
        m_scheduleData  = nullptr;
    AbstractData*
        m_absData       = nullptr;
    Customer*
        m_customer      = nullptr;
    bool
        m_running       = true;
    bool
        m_mlPaused      = false;
    
};

class AlgorithmHelper final {
    AlgorithmHelper()  = delete;
    ~AlgorithmHelper() = delete;
public:
    /*[[nodiscard]]*/ static void
        runWithAlgorithmDij(
            BackendBase*    instance,
            QString&        fromCity,
            QString&        destCity,
            int             startT,
            int             timeLmt = 0x7FFFFFF
        );
};

#endif // BACKENDBASE_H
