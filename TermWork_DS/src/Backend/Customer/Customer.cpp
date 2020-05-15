#include "Customer.h"

void Customer::addTask(Task&& task) {
    m_taskQueue.push(std::forward<Task&&>(task));
}

void Customer::addTask(const Task& task) {
    m_taskQueue.push(task);
}

void Customer::updateStatus() {
    if(!m_taskQueue.empty()) {
        if(m_sysTime/60 == m_taskQueue.front().startTime) {
            Task& task = m_taskQueue.front();
            
            QString log("旅客在【");
            log += task.fromCity + "】于 ";
            log += QString::number(task.startTime/60);
            log += ":";
            log += QString::number(task.startTime%60);
            switch (task.behaviorType) {
            case Waiting:
                log += " 开始等候【";
                if(task.vehicleType == 0) {
                    log += "A";
                } else if(task.vehicleType == 1) {
                    log += "T";
                } else {
                    log += "C";
                }
                log += QString::number(task.tacNumber);
                log += "】发车。发车时间：";
                log += QString::number(task.endTime/60 % 24);
                log += ":";
                log += QString::number(task.endTime%60);
                break;
            case Traveling:
                log += " 乘坐【";
                if(task.vehicleType == 0) {
                    log += "A";
                } else if(task.vehicleType == 1) {
                    log += "T";
                } else {
                    log += "C";
                }
                log += QString::number(task.tacNumber);
                log += "】驶向【" + task.destCity;
                log += "】。预计到达时间：";
                log += QString::number(task.endTime/60 % 24);
                log += ":";
                log += QString::number(task.endTime%60);
            }
            qDebug() << log;
            
            emit posChanged(
                task.fromCity,
                task.destCity,
                ((task.endTime + (24*60)) - task.startTime) % (24*60)
            );
            
            m_taskQueue.pop();
            m_logger->InsertLogInfo(log, eEVENT_LEVEL::LV_EVENT);
        }
        m_sysTime = (m_sysTime + 36) % (24*60*60);
//        qDebug() << m_sysTime;
    }
}
