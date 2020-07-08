#include "BackendBase.h"
#include "Customer/Customer.h"

#include <queue>
#include <cmath>
#include <vector>
#include <limits>
#include <iostream>

constexpr int INF       = 0x3F3F3F3F;
constexpr int RISK[3]   = {9, 5, 2};

// 修改为QHash<QString, absCity&> 和 QList/vector<abscity>

struct Behavior {
    int waitTime;
    int tacNum;
    int travelType;
    int risk;
    int startTime;
};

// Safety Refered Algorithm
void AlgorithmHelper::runWithAlgorithmDij(
    BackendBase*    instance,
    QString&        fromCity,
    QString&        destCity,
    int             startT,
    bool            setoff,
    int             timeLmt
) {
    using std::vector;
    using std::queue;
    
    int startCityId =
        instance->m_absData->m_cityHash[fromCity].id;
    int destCityID =
        instance->m_absData->m_cityHash[destCity].id;
//-----------------------Dij------------------------------//
    const size_t cityNum = instance->m_absData->m_cityHash.size();
    
    vector<bool> visit(cityNum, false);
    vector<int>  risks(cityNum, INF);
    vector<int>  prev(cityNum, -1);
    vector<vector<int>>  
        spendTime(cityNum);
    for(size_t i = 0; i < cityNum; i++) {
        spendTime[i].resize(cityNum);
        if(i == static_cast<size_t>(startCityId)) {
            spendTime[i] = vector<int>(cityNum, startT);
        }
    }
    vector<vector<int>>
        routeIndex(cityNum);
    for(auto& vec: routeIndex) {
        vec.resize(cityNum);
    }
//    vector<int>  routeIndex(cityNum, -1);
    
    
    int curCity             = startCityId;
    risks[curCity]          = 0;
    visit[curCity]          = true;
    
//    for(auto& route: instance->m_absData->m_routeMap[curCity]) {
    {
        int prevTime = startT;
        for(size_t i = 0, size = instance->m_absData->m_routeMap[curCity].size();
            i < size; i++)
        {
            AbstractRoute& route = instance->m_absData->m_routeMap[curCity][i];
            int waitTime = 
                ((route.startTime + (24*60)) -
                 prevTime) % (24*60);
            
            if(waitTime + route.costTime > timeLmt) {
                continue;
            }
            
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
        
//        prevFinTime[curCity] = 
//            instance->m_absData->m_routeMap[prev[curCity]]
//                [routeIndex[prev[curCity]][curCity]].costTime +
//                prevFinTime[prev[curCity]];
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
            
            if(waitTime + prevTime + route.costTime - startT > timeLmt) {
                continue;
            }
            
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
    
    std::vector<int> routePass(cityNum, -1);
    size_t index = cityNum - 1;
    
    int pos = destCityID;
    if(!visit[pos]) {
        emit instance->sigNewMessage(
            instance->m_sysTime.toString("[yyyy-MM-dd HH:mm]") +
                "No such path found"
        );
    } else {
        QString info {};
        while(prev[pos] != -1) {
            info += instance->m_absData->m_cityIdHash[pos].name + "<-";
            
            routePass[index] = pos;
            index--;
            
            pos = prev[pos];
        }
        info += instance->m_absData->m_cityIdHash[startCityId].name;
        qDebug() << info;
///---------------------------------------------------------------------------------//
        
        routePass[index] = startCityId;
        
        QString result {};
        int taskStartTime = startT;
        while(index < cityNum - 1) {
            int cityId   = routePass[index];
            int nextCity = routePass[index + 1];
            int routeId  = routeIndex[cityId][nextCity];
            AbstractRoute& route = instance->m_absData->m_routeMap[cityId][routeId];
            
            result += "\n\t在【";
            result += instance->m_absData->m_cityIdHash[cityId].name;
            result += "】乘坐";
            if(route.vehicleType == 0) {
                result += "A";
            } else if(route.vehicleType == 1) {
                result += "T";
            } else if(route.vehicleType == 2) {
                result += "C";
            }
            result += QString::number(route.serialNum);
            
            qDebug()<< "城市信息" 
                    << cityId 
                    << routeId;
            qDebug()<< "选择路线信息"
                    << route.costTime
                    << route.destCity
                    << route.serialNum
                    << route.startTime
                    << route.vehicleType
                    << "\n";
//------------------若选择出发，则给旅客添加任务-----------------------//
            if(setoff) {
            instance->m_customer->addTask(
                Task(
                    taskStartTime,
                    spendTime[cityId][nextCity] - route.costTime,
                    Customer::Waiting,
                    route.serialNum,
                    route.vehicleType,
                    instance->m_absData->m_cityIdHash[cityId].name,
                    instance->m_absData->m_cityIdHash[cityId].name
                )
            );
            instance->m_customer->addTask(
                Task(
                    spendTime[cityId][nextCity] - route.costTime,
                    spendTime[cityId][nextCity],
                    Customer::Traveling,
                    route.serialNum,
                    route.vehicleType,
                    instance->m_absData->m_cityIdHash[cityId].name,
                    instance->m_absData->m_cityIdHash[nextCity].name
                )
            );
            instance->m_customer->addTask(
                Task(
                    0,
                    spendTime[cityId][nextCity],
                    Customer::Arrive,
                    0,
                    0,
                    instance->m_absData->m_cityIdHash[nextCity].name,
                    instance->m_absData->m_cityIdHash[nextCity].name
                )
            );
            taskStartTime = spendTime[cityId][nextCity];
            }
            
            index++;
        }
        
        result += "\n\t总耗时:" +
                    QString::number
                        ((spendTime[prev[destCityID]][destCityID] - startT) / 60) + "小时" +
                    QString::number
                        ((spendTime[prev[destCityID]][destCityID] - startT) % 60) + "分钟";
        result += "\n\t总危险值:" + QString::number(risks[destCityID]);
        
        emit instance->sigNewMessage(
            instance->m_sysTime.toString("[yyyy-MM-dd HH:mm]") +
                result
        );
    }
}
