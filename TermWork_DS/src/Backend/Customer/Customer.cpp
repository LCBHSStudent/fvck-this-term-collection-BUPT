#include "Customer.h"

void Customer::addTask(Task&& task) {
    m_taskQueue.push(std::forward<Task&&>(task));
}

void Customer::addTask(const Task& task) {
    m_taskQueue.push(task);
}

void Customer::updateStatus(QString& log) {
    if(!m_taskQueue.empty()) {
        while(m_sysTime/60 == m_taskQueue.front().startTime) {
            Task& task  = m_taskQueue.front();
            if(task.behaviorType == Customer::Arrive) {
                log =  "旅客到达【" + task.destCity + "】, ";
                log += "当前时间" + QString::number(task.endTime/60%24);
                log += ":";
                log += QString::number(task.endTime%60);
                emit statusChanged(
                    task.fromCity,
                    task.destCity,
                    task.endTime - task.startTime,
                    Arrive
                );
            } else {
                log = "旅客在【";
                log += task.fromCity + "】于 ";
                log += QString::number(task.startTime/60%24);
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
                    log += QString::number(task.endTime/60%24);
                    log += ":";
                    log += QString::number(task.endTime%60);
                    emit statusChanged(
                        task.fromCity,
                        task.destCity,
                        task.endTime - task.startTime,
                        Waiting
                    );
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
                    log += QString::number(task.endTime/60%24);
                    log += ":";
                    log += QString::number(task.endTime%60);
                    emit statusChanged(
                        task.fromCity,
                        task.destCity,
                        task.endTime - task.startTime,
                        Traveling
                    );
                    break;
                    
                default:
                    break;
                }
            }
            m_taskQueue.pop();
            m_logger->InsertLogInfo(log, eEVENT_LEVEL::LV_EVENT);
        }
        m_sysTime += 36;
//        qDebug() << m_sysTime;
    }
}
