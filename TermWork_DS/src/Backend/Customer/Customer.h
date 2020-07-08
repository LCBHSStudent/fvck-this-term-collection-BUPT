#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QObject>
#include <QSqlDatabase>
#include <QDateTime>

#include <queue>

#include "Logger.h"

class Task {
public:
    Task(
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
        
    int     startTime;
    int     endTime;
    int     behaviorType;
    int     tacNumber;
    int     vehicleType;
    QString fromCity;
    QString destCity;
};


class Customer final: public QObject {
    Q_OBJECT
public:
    explicit Customer(int _time):
        m_sysTime(_time)
    {
        m_logger = new Logger;
    }
    ~Customer() {
        m_logger->m_bStart = false;
        m_logger->wait();
        
        delete m_logger;
        m_logger = nullptr;
    }
    
    enum BehaviorType {
        Waiting = 0,
        Traveling,
        Arrive,
    };
    
signals:
    void 
        posChanged(
            QString from,
            QString dest,
            qint64  duration
        );
    
public:
    void
        setSysTime(int time) {
            m_sysTime   = time;
            m_dayPassed = false;
        }
    void
        addTask(Task&&);
    void
        addTask(const Task&);
    void
        updateStatus(QString&);
    bool
        isTaskEmpty() const {return m_taskQueue.empty();}
    
private:
    int
        m_sysTime;
    bool
        m_dayPassed;
    Logger*
        m_logger = nullptr;
    std::queue<Task>
        m_taskQueue = {};
};

#endif // CUSTOMER_H
