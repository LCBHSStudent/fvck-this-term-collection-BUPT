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

// 装饰品，个人习惯
#define FUNCTION 
#define RESOURCE 
#define Q_RESOURCE 

class BackendBase : public QQuickItem {
    Q_OBJECT
    
    friend class AlgorithmHelper;   // 辅助友元类，用于计算最短路径
    // 供GUI使用的城市modelData
    Q_PROPERTY(
        CityModel*  cityModel
        READ        getCityModel
        NOTIFY      cityModelChanged)
    // 与GUI同步的系统时间展示
    Q_PROPERTY(
        QString     sysTime
        READ        getSysTime
        NOTIFY      sysTimeChanged)
    // 供GUI使用的时刻表modelData
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
    // 定义旅行工具&策略枚举并暴露给GUI使用
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
        slotLoadData();             // 从数据库读取城市以及时刻表信息
    void
        invokePauseMainThrd();      // 用于GUI方打开侧边栏操作时冻结后台系统时间
    void
        invokeContinueMainThrd();   // 用于GUI方关闭侧边栏时解冻后台时间
    void                            // 暴露给GUI的查询接口
        slotStartQuery(
            QString     fromCity,
            QString     destCity,
            int         startH,
            int         startM,
            int         policy,
            bool        setoff,
            int         timeLmt = 0x3F3F3F3F
        );
    // 用于暴露数据modelData给GUI
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
    void                                    // 提示GUI旅客位置变化的信号
        slotCustomerPosChanged(
            QString from,
            QString dest,
            qint64  duration
        );
    
signals:    // 信号，字面义
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
    void
        sigPopupMessage(QString msg);
    
    // 供后台类自身工作调用的私有函数实现
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
    CityModel*                      // 供GUI使用的城市数据
        m_cityData      = nullptr;
    ScheduleModel*                  // 供GUI使用的时刻表数据
        m_scheduleData  = nullptr;
    AbstractData*                   // 供寻路策略算法使用的抽象数据
        m_absData       = nullptr;
    Customer*                       // 全局唯一旅客
        m_customer      = nullptr;
    bool                            // 系统是否存活 (生命周期至窗口关闭)
        m_running       = true;
    bool                            // 系统是否未被冻结
        m_mlPaused      = false;
};

// 算法辅助类，只包含一个静态函数，只提供接口
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
            bool            setoff,
            int             timeLmt = 0x7FFFFFF
        );
};

#endif
