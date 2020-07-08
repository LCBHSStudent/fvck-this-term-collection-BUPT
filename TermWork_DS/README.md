# Conv-Travel-Emulator 北京邮电大学2018级 数据结构课程设计项目
## 目录
1. 设计任务的描述
2. 功能需求说明及分析
3. 总体方案设计说明
4. 数据结构说明与数据字典(数据名称 & 用途)
5. 范例执行结果及测试情况说明
6. 评价和改进意见
7. 用户使用说明

## 1. 设计任务的描述
&emsp;&emsp;本项目是由数据结构课程设计中要求下的COVID-19疫情环境下低风险旅行模拟系统的设计，模拟一个虚拟的城市交通网络环境，城市之间有各种交通工具（汽车、火车和飞机）相连，有些城市之间无法直达，需要途径中转城市。旅客于某一时刻向系统提出旅行要求。考虑在当前COVID-19疫情环境下，各个城市的风险程度不一样，分为低风险、中风险和高风险三种。系统根据风险评估，为该旅客设计一条符合旅行策略的旅行线路。  
&emsp;&emsp;**综上所述，这是一个由城市交通数据与疫情相关危险系数组成的系统：用户向系统提出请求，设置旅行的出发地及目的地，系统能够以可视化的方式进行反馈并进行模拟旅行。**
## 2. 功能需求说明及分析
### 功能需求说明

#### 该系统需要建立完备的城市、路线、交通工具信息配置 
&emsp;&emsp;1. 城市总数不少于10个，为不同城市设置不同的单位时间风险值：低风险城市为0.2；中风险城市为0.5；高风险城市为0.9。各种不同的风险城市分布要比较均匀，个数均不得小于3个。旅客在某城市停留风险计算公式为：*旅客在某城市停留的风险=该城市单位时间风险值\*停留时间。*  
&emsp;&emsp;2. 建立汽车、火车和飞机的时刻表（航班表），假设各种交通工具均为起点到终点的直达，中途无经停。需要注意的是：**不能太简单，城市之间不能总只是1班车次；整个系统中航班数不得超过10个，火车不得超过30列次；汽车班次无限制；**

#### 该系统需要提供为旅客选择旅行路线的低风险策略
&emsp;&emsp;1. 最少风险策略：无时间限制，风险最少即可  
&emsp;&emsp;2. 限时最少风险策略：在规定的时间内风险最少  
&emsp;&emsp;3. 为不同交通工具设置不同单位时间风险值，交通工具单位时间风险值分别为：汽车=2；火车=5；飞机=9。  
&emsp;&emsp;&emsp;&emsp;旅客乘坐某班次交通工具的风险 = 该交通工具单位时间风险值*该班次起点城市的单位风险值*乘坐时间。  

#### 该系统需要提供模拟旅行机制（时间系统以及随时间变化的状态系统）
&emsp;&emsp;1. 旅行模拟系统以时间为轴向前推移，每10秒左右向前推进1个小时(非查询状态的请求不计时，即：有鼠标和键盘输入时系统不计时)  
&emsp;&emsp;2. 用图形绘制地图，并在地图上实时反映出旅客的旅行过程。
#### 该系统需要提供日志功能
&emsp;&emsp;建立日志文件，对旅客状态变化和键入等信息进行记录

### 功能需求分析
&emsp;&emsp;本项目需要实现以下几个模块：  
&emsp;&emsp;&emsp;&emsp;1. 附带GUI的交互系统：能显示系统信息（包括城市、路线、旅客状态、系统消息等），并处能理用户的输入  
&emsp;&emsp;&emsp;&emsp;2. 算法模块：实现最小风险查询策略（包括扩展策略，计算交通工具乘坐风险）  
&emsp;&emsp;&emsp;&emsp;3. 日志模块：将旅客状态变化，用户输入记录至文件中
## 3. 总体方案设计说明
&emsp;&emsp;基于[2]中分析，本项目所实现各个功能的具体设计如下:  
1. GUI界面：使用QtQuick搭建，基于风格化的自定义组件包括侧边栏菜单、复选框、按钮、文本框、Model-View信息组件、旅客进程显示粒子效果等；通过组合控件实现对用户的输入处理 & 反馈直观的可视化系统信息到用户，注意通过分块文件降低UI项目的耦合性
2. 使用带剪枝的类迪杰斯特拉最短路径算法（每一步选择会基于上一步与这一步的旅行时间计算风险系数）来查找最短路径&有时间限制的最短路径
3. 使用SQLite数据库存储系统信息，并建立log类将日志输出打印至另一个独立的数据库  

&emsp;&emsp;故本项目在完成后的文件目录结构如下：  
|——TermWork_DS.pro (QtCreator的项目文件)  
|——qml.qrc (qt_resource文件，用于将qml文件编译为二进制至exe文件中)  
|——res.qrc (qt_resource文件, 用于将图片与UI素材等资源文件编译为二进制至exe文件中)  
|——qtquickconrtols2.conf (本项目使用了QtQuick的Universal组件风格，属于配置文件)  
|——TermWork_DS.pro.user (生成的供vs使用的项目文件)  
|——/android/ (做着玩的android移植，包括AndroidManifest和gradle等配置文件)  
|——/build/ (项目构建目录)  
|——/qml/ (qml文件根目录)  
|————main.qml (GUI根组件，创建自定义主窗体&注册后台事务处理类以及一些其他的组件&信号处理)  
|————/Components/ (一些UI组件的目录)  
|——————OneBtnToast.qml (带按钮的弹窗组件)  
|——————UIBase.qml (UI基组件，包含DPI屏幕适配，tintColor，窗口信号预定义变量等)  
|————/Page/ (页面view的组件目录)  
|——————DataVisualizationPage.qml (展示系统信息：车辆班次，城市信息并提供一定检索服务的页面)  
|——————MainDrawerPage.qml (主页面的侧拉栏菜单，展示系统消息，提供路线查询信息输入输出组件)  
|——————OverviewPage.qml (主页面，提供地图展示，旅客位置变化显示等功能)  
|——/res/  (资源文件目录)  
|————... (内容略)  
|——/src/ (源文件目录)  
|————main.cpp (仅提供QtQuick程序入口位点功能)  
|————/Backend/ (相对于前台GUI的后台事务处理代码，即逻辑代码)  
|——————AlgorithmHelper.cpp (查询路径算法的实现，由于长度问题单独列出，其本身也是Backend类的辅助类实现)  
|——————BackendBase.cpp & ~.h (后台类的实现，主要功能为存储系统数据、处理由GUI发来的用户输入和向GUI端传送系统消息&状态变化信息等)  
|——————/Customer/ (包含旅客 & log类的实现源码)  
|————————Customer.cpp & Customer.h (旅客类的实现)  
|————————Logger.cpp & Logger.h (日志类的实现，用于打印旅客状态变化信息到数据库)  
|——————/DataModel/ (包含存储系统数据的类实现源码)  
|————————AbstractData.cpp & ~.h (实现供后台系统使用的路线信息&城市信息类)  
|————————CityModel.cpp & ~.h (利用QAbstractModel实现可供前端查询并用Model-View模式显示的城市数据结构类)  
|————————ScheduleModel.cpp & ~.h (实现与作用铜CityModel的交通工具时刻表数据结构类)  

### 核心Backend类定义，通过QmlRegisterType注册为Qml类后在Qml中创建唯一实例
```c++
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
```
通过Backend的主事务循环，本系统可以进行系统时间的推进，旅客状态的变更，具体实现如下：（这项事务是跑在单独线程中的）
```c++
void BackendBase::mainLoop() {
    uint8_t ten = 0;
    while(m_running) {      // 如果系统仍存活
        while(m_mlPaused && m_running)      // 如果被操作冻结且系统存活则阻塞
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if(ten == 10) {
            m_sysTime =  m_sysTime.addSecs(360);        // 每秒系统时间增加6分钟
            emit sysTimeChanged();                      // 即十秒流动一小时
            ten = 0;
        }
        QString log;
        m_customer->updateStatus(log);                  // 为了旅客任务的精确性，每次循环中都检查旅客状态
        if(log.length()) {
            emit sigNewMessage(m_sysTime.toString("[yyyy-MM-dd HH:mm] ") + log);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(99)); // 该线程睡眠99ms
        ten++;      // 每一秒激活一次内部循环，推进系统时间
    }
}
```
&emsp;&emsp;其中旅客更新自身状态是基于起始的系统时间与自身配备的的时间（提高精度，防止由于运算导致的不触发任务）  
&emsp;&emsp;核心代码如下
```c++
void Customer::updateStatus(QString& log) {
    // 若任务队列不为空，则进入循环
    if(!m_taskQueue.empty()) {
        // 由于旅客的systime精度较高，需要取余
        qDebug() << m_sysTime / 60 << m_taskQueue.front().startTime%(24*60);
        if(m_sysTime/60 == m_taskQueue.front().startTime%(24*60)) {
            Task& task  = m_taskQueue.front();
            if(task.behaviorType == Customer::Arrive) {
                // log
            } else {
                // log
                switch (task.behaviorType) {
                case Waiting:
                    // log
                    break;
                case Traveling:
                    // log
                    emit posChanged(
                        task.fromCity,
                        task.destCity,
                        ((task.endTime + (24*60)) - task.startTime) % (24*60)
                    );
                    break;
                    
                default:
                    break;
                }
            }
            m_taskQueue.pop();
            m_logger->InsertLogInfo(log, eEVENT_LEVEL::LV_EVENT);
        }
        m_sysTime = (m_sysTime + 36) % (24*60*60);  // 移动旅客时间
    }
}
```

使用带剪枝的迪杰斯特拉算法求出最短路径，算法核心部分具体实现如下：
```c++
    // -----------------------Dij------------------------------ //
    // 初始化
    const size_t cityNum = instance->m_absData->m_cityHash.size();
    
    vector<bool> visit(cityNum, false);     // 该城市已被访问？
    vector<int>  risks(cityNum, INF);       // 记录到某城市的最小风险值
    vector<int>  prev(cityNum, -1);         // 记录城市的前驱节点
    vector<vector<int>>  
        spendTime(cityNum);                 // 记录从一个城市到另一个城市后的系统时间值
    for(size_t i = 0; i < cityNum; i++) {
        spendTime[i].resize(cityNum);
        if(i == static_cast<size_t>(startCityId)) {
            spendTime[i] = vector<int>(cityNum, startT);
        }
    }
    vector<vector<int>>
        routeIndex(cityNum);                // 记录一个城市到另一个城市的路径选择值
    for(auto& vec: routeIndex) {
        vec.resize(cityNum);
    }
    
    // -------------------------DIJ 初次循环初始化 ------------------------ //
    int curCity             = startCityId;
    risks[curCity]          = 0;
    visit[curCity]          = true;
    
    {
        int prevTime = startT;
        for(size_t i = 0, size = instance->m_absData->m_routeMap[curCity].size();
            i < size; i++)
        {
            AbstractRoute& route = instance->m_absData->m_routeMap[curCity][i];
            int waitTime = 
                ((route.startTime + (24*60)) -
                 prevTime) % (24*60);
            
            // 若时间超过则剪枝
            if(waitTime + route.costTime > timeLmt) {
                continue;
            }
            // 根据路径时间评估风险值
            double risk =
                static_cast<double>(waitTime) / 60 *
                instance->m_absData->m_cityIdHash[curCity].factor;
            
            risk += route.costTime / 60 * RISK[route.vehicleType];
            
            if(static_cast<int>(risk) < risks[route.destCity]) {
                risks[route.destCity] = static_cast<int>(risk);
                routeIndex
                    [curCity][route.destCity] =
                        static_cast<int>(i);
                
                spendTime[curCity][route.destCity] =
                    prevTime + waitTime + route.costTime;
                prev[route.destCity] = curCity;
                qDebug() << risks[route.destCity] << " " << i;
            }
        }
    }
    
    
    for(size_t i = 0; i < cityNum; i++) {
        // 取风险最小城市扩展节点
        int minn = INF;
        int temp = curCity;
        for(size_t j = 0; j < cityNum; j++) {
            if(!visit[j] && risks[j] < minn) {
                minn    = risks[j];
                curCity = static_cast<int>(j);
                qDebug() << "扩展城市结点" 
                         << instance->m_absData->m_cityIdHash[(int)j].name;
            }
        }
        if(curCity == temp) {
            break;
        }
        visit[curCity]  = true;

        int prevTime = spendTime[temp][curCity];
        
        if(curCity == destCityID)
            break;
        
        for(size_t i = 0, size = instance->m_absData->m_routeMap[curCity].size();
            i < size; i++)
        {
            AbstractRoute& route = instance->m_absData->m_routeMap[curCity][i];
            if(visit[route.destCity]) {
                continue;
            }
            int waitTime = 
                ((route.startTime + (24*60)) -
                 prevTime) % (24*60);
            // 若时间超过则剪枝
            if(waitTime + prevTime + route.costTime - startT > timeLmt) {
                continue;
            }
            // 根据路径时间评估风险值
            double risk =
                static_cast<double>(waitTime) / 60.0f *
                instance->m_absData->m_cityIdHash[curCity].factor;
            
            risk += route.costTime / 60.0f * RISK[route.vehicleType];
            
            if(static_cast<int>(risk) < risks[route.destCity]) {
                risks[route.destCity] = static_cast<int>(risk);
                routeIndex
                    [curCity][route.destCity] =
                        static_cast<int>(i);
                
                spendTime[curCity][route.destCity] = prevTime + waitTime + route.costTime;
                prev[route.destCity] = curCity;
                qDebug() << risks[route.destCity] << i;
            }
        }
    }
```

## 4. 数据结构说明与数据字典
### 交通信息(时刻表)-数据库表设计
|  TACNumber   | FromStation  |  ToStation   | StartH  |  StartM   | ArriveH  |  ArriveM   | TransportationType  |
|  ----  | ----  |  ----  | ----  |  ----  | ----  |  ----  | ----  |
| int  | string | string  | int | int  | int | int  | enum(int) |
※ TACNumber：&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;交通工具序号，主要用于辨识  
※ From/ToStation：&nbsp;&nbsp;数据库中利用文本方式，直观地标记出发地与目的地  
※ Start/Arrive H/M：记录该列车始发与到达的时间信息，ArriveH需要对24取余，每增加一个24即为行程增加一天  
※ TransportationType：记录该路线的交通工具种类
### 城市信息-数据库表设计
|  CityID  |  CityName  |  CityDangerFactor  |
|  ----    |  ----      |  ----              |
|  int     |  string    |  float32           |
※ CityID：城市序号，用于索引  
※ CityName：城市名，用于索引 & hash  
※ CityDangerFactor：城市危险系数，用于计算路线危险度  
### 用于程序的数据结构 (主要提两个AbstractDataType)
#### 城市信息AbstractCity定义
```c++
class AbstractCity final {
public:
    AbstractCity() = default;
    AbstractCity(int _id, float _factor):
        id(_id), factor(_factor) {}
    AbstractCity(QString& _name, float _factor):
        factor(_factor), name(_name)  {}
    AbstractCity(QString& _name, int _id, float _factor):
         id(_id), factor(_factor), name(_name)  {}
    
public RESOURCE:
    int     id      = -1;       // 城市ID
    float   factor  = 0.0;      // 城市滞留危险因子
    QString name    = {};       // 城市名称
};
```
#### 路径信息AbstractRoute定义
```c++
class AbstractRoute final {
public:
    AbstractRoute(
        int type,
        int dest,
        int start,
        int cost,
        int serial
    ):
        vehicleType(type),
        destCity(dest),
        startTime(start),
        costTime(cost - start),
        serialNum(serial) {}
    
public RESOURCE:
    int     vehicleType;        // 交通工具类型
    int     destCity;           // 目的城市ID
    int     startTime;          // 起始时间
    int     costTime;           // 消耗时间 (所用时长)
    int     serialNum;          // 交通工具编号
};
```
#### 以上两者的聚合，供Backend和AlgorithmHelper使用
```c++
class AbstractData final {
    friend class BackendBase;
    friend class AlgorithmHelper;
public:
     AbstractData() = default;
    ~AbstractData() = default;

    void
        printAllRouteInfo(QString& cityName);       // 用于Debug
    
private:
     std::vector<
        std::vector<AbstractRoute>
     >                                      // 使用二维vector模拟邻接表记录路径信息的图数据结构
        m_routeMap {};
     QHash<QString, AbstractCity>           // 提供城市名到城市信息的hash
        m_cityHash {};
     QHash<int, AbstractCity>               // 提供城市id到城市信息的hash
        m_cityIdHash {};
};
```
#### 旅客任务Task定义
```c++
class Task {
public:
    Task(                                   // 构造函数
        int      _startTime,
        int      _endTime,
        int      _behaviorType,
        int      _tacNumber,
        int      _vehicleType,
        QString& _fromCity,
        QString& _destCity
    ): startTime(_startTime),
       endTime(_endTime), 
       behaviorType(_behaviorType),
       tacNumber(_tacNumber),
       vehicleType(_vehicleType),
       fromCity(_fromCity), 
       destCity(_destCity) {}
        
    int     startTime;                      // 任务开始时间
    int     endTime;                        // 任务结束时间
    int     behaviorType;                   // 行为类型（等待——移动——到达）
    int     tacNumber;                      // 交通工具班次
    int     vehicleType;                    // 交通工具类型 (汽车火车飞机)
    QString fromCity;                       // 出发城市
    QString destCity;                       // 目的城市
};
```
### 旅客类定义
```c++
class Customer final: public QObject {
    Q_OBJECT
public:
    explicit Customer(int _time):
        m_sysTime(_time)
    {
        m_logger = new Logger;          // 构造，添加日志组件
    }
    ~Customer() {
        m_logger->m_bStart = false;
        m_logger->wait();
        
        delete m_logger;
        m_logger = nullptr;
    }
    
    enum BehaviorType {                 // 行为类型枚举
        Waiting = 0,
        Traveling,
        Arrive,
    };
    
signals:
    void                                // 旅客位置改别信号，用于和GUI交互
        posChanged(
            QString from,
            QString dest,
            qint64  duration
        );
    
public:
    void                                // 用于初始化旅客状态时间
        setSysTime(int time) {
            m_sysTime   = time;
            m_dayPassed = false;
        }
    void                                // 添加旅客任务
        addTask(Task&&);
    void                                // 重载2
        addTask(const Task&);
    void                                // ※随时间更新旅客状态
        updateStatus(QString&);
    bool                                // 判断旅客是否已在旅行
        isTaskEmpty() const {return m_taskQueue.empty();}
    
private:
    int                                 // 旅客独立的处理时间
        m_sysTime;
    bool
        m_dayPassed;
    Logger*                             // 日志类
        m_logger = nullptr;
    std::queue<Task>                    // 任务队列
        m_taskQueue = {};
};
```
## 5. 范例执行结果及测试情况说明
### 本条目中，将测试范例与测试条件分为*简单模式下*（仅保留火车飞机）与*复杂模式*下（引入大量汽车路径）的*一般测试用例*与普遍情况下的*特殊测试用例*；其中*一般测试用例*指**使用正常规范的输入数据测试程序**，*特殊测试用例*是指**采用不合法的或者较边缘的输入数据测试程序**。
#### 1. 简单模式x一般测试用例

#### 2. 复杂模式x一般测试用例

#### 3. 特殊测试用例

## 6. 评价和改进意见
截至本条目编写时，项目设计中预计完成的任务完成情况如下：  

- [v] 全部实现了旅行策略规划要求：包括按等待时间和乘坐交通工具时间计算风险值
- [v] 设计Universal风格的GUI，提供了良好的视觉体验
- [v] 实现了在打开侧拉操作栏后的系统时间冻结与界面旅客粒子动画冻结的feature
- [v] 前端与后端分离，降低项目组件耦合性
- [v] 优化c++代码结构，提高密致性的同时降低依赖性
- [v] 基于本系统提供可靠的能够执行的算法实现
- [v] 按照设计要求录入路线时刻表信息，使用数据库，简化人工操作
- [v] 提供了Log类，打印旅客状态变化为字符串到/Log/Log.db中
- [v] 对用户输入做了边缘检测，能够直接提示消除一些不规范操作
- [v] 可直接在系统GUI中进行系统信息的查询
- [v] 参照现实世界并挑选生成的火车&飞机时刻表 & 参照现实时间度量随即加入的大量汽车时刻表
- [x]
- [x] 仍有一些地方用户体验做的不够到位：输入限时单位为分钟
- [x] 代码注释不够完备
- [x] 部分类设计为final，二次开发较为麻烦
- [x] 旅客动效不够醒目，并且不能明显从地图界面的体现出旅客等待状态
- [x] GUI中的后台信息界面文字排版不够美观
- [x] 为了与GUI部分共享，一部分数据被重复加入内存，提高了内存占用
- [x] 数据结构的内存占用仍可优化
- [x] GUI中对城市的坐标仍处于写死的状态，需要扩展显示城市支持只能替换qml文件，不够灵活

## 7. 用户使用说明
&emsp;&emsp;尊敬的用户您好，欢迎使用**Conv-Travel-Emulator：疫情旅行模拟系统**，本系统提供良好的图形用户界面与操作体验，使用开始前，您需要了解本系统所提供的功能：  
&emsp;&emsp;1. 通过选择始发地与到达地，本系统可依据您提供的信息查询出一条最符合要求的路径（如果存在的话）  
&emsp;&emsp;2. 通过选择旅行策略，本系统能够分别依据是否有时间限制为您查询出风险最小的路径  
&emsp;&emsp;3. 通过程序侧拉菜单内的消息框，您可以直观的获取旅客当前的状态信息  
&emsp;&emsp;4. 通过程序主界面的地图，您可以看到由粒子效果表示的旅客运行状态轨迹  
&emsp;&emsp;5. 通过程序二级数据界面，您可以察看受支持的城市列表与其危险度信息和交通网信息，并按类型或出发地与到达地进行表筛选  

&emsp;&emsp;运行文件夹内的TermWork_DS.exe，进入程序主界面。动画后您将看到中国地图与用粒子动效标记出的支持城市位置与右下角的系统当前时间。这个时间取决于您启动程序时，计算机的真实时间，并依照每秒前进6分钟的比例尺递增。  
&emsp;&emsp;窗体的左上角是主要控制部件侧拉窗口的开关，点击这个按钮或者鼠标从左侧边缘向右拖拽都能够打开此控制台。控制台最上方一行可供选择出发地与到达地；下面部分从左到右分别是策略选择复选框：提供无时间限制的危险度最低策略和有时间限制的危险度最低策略；出发时间（小时，秒钟）与时间限制（单位：分钟，只在选中时间优先策略时有效且可编辑）；“现在出发”勾选框：勾选后，旅客会根据查询结果进入旅行状态直到结束，若取消勾选，则只会将路线信息打印至下方的文本区域中；“GO!”按钮：根据上方勾选框的状态执行查询操作或旅客出发操作。此外，若产生了不合法的输入，会弹窗提示您需要重新检查并纠正输入。  
&emsp;&emsp;在打开左侧控制台窗口后，系统时间将被冻结，正在旅行中的旅客状态不会发生变化，界面中的旅客粒子也会保持进度而停留。中间的区域为消息展示区，主要展示路径查询信息与旅客状态变化的消息，可以使用滚轮或长按上下滑动查看历史消息，消息前的时间戳为该系统内的系统时间点。  
&emsp;&emsp;控制台窗口左下角的按钮是进入系统详细信息界面的按钮，进入后，左侧的列表中显示支持的城市与其危险系数信息，右侧的列表则是所有交通工具的时刻表。右侧列表为您提供了筛选功能，右上角的点击式下拉菜单栏中可以选择显示的交通工具种类；左下和右下的文本输入框分别可以检索特定出发地&目的地的交通工具时刻信息，输入完成后按回车即可看到效果，为空白时不做任何筛选。**需要注意的是，此时系统时间的冻结将被解除。**  
&emsp;&emsp;旅客按照查询到的路径出发后，可以在地图上确认旅客乘坐交通工具的进度（使用粒子效果发出射线，从出发地到目的地，长度与旅客已乘坐的时间占该车次总时间的百分比正相关）  
&emsp;&emsp;程序首次运行后，会在运行目录下生成.db文件，用户可以通过更改其中的城市表，时刻表自由操作系统信息，在下一次启动程序并读取完数据后立即生效；程序运行时会新建一个/Log/文件夹，其中的.db文件记录了包含系统时间的旅客的状态变化信息和现实世界时间戳，可以直观读取。  
&emsp;&emsp;以上就是**Conv-Travel-Emulator：疫情旅行模拟系统**的各项功能以及各个组件的使用方式，希望能够对您有所帮助！