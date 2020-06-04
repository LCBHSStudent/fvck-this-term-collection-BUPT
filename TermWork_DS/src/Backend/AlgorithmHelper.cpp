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
    vector<int>  prevFinTime(cityNum, 0);
    vector<vector<int>>
        routeIndex(cityNum);
    for(auto& vec: routeIndex) {
        vec.resize(cityNum);
    }
//    vector<int>  routeIndex(cityNum, -1);
    
    
    int curCity           = startCityId;
    prevFinTime[curCity]  = startT;
    
    risks[curCity]          = 0;
    visit[curCity]          = true;
    
//    for(auto& route: instance->m_absData->m_routeMap[curCity]) {
    {
        int prevTime = prevFinTime[curCity];
        for(size_t i = 0, size = instance->m_absData->m_routeMap[curCity].size();
            i < size; i++)
        {
            AbstractRoute& route = instance->m_absData->m_routeMap[curCity][i];
            int waitTime = 
                ((route.startTime + (24*60)) -
                 prevTime) % (24*60);
            double risk =
                static_cast<double>(waitTime) / 60 *
                instance->m_absData->m_cityIdHash[curCity].factor;
            
            risk += route.costTime / 60 * RISK[route.vehicleType];
            
            if(static_cast<int>(risk) < risks[route.destCity]) {
                risks[route.destCity] = static_cast<int>(risk);
                routeIndex
                    [curCity][route.destCity] =
                        static_cast<int>(i);
                
                prevFinTime[curCity] = prevTime + waitTime;
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
        
        prevFinTime[curCity] = 
            instance->m_absData->m_routeMap[prev[curCity]]
                [routeIndex[prev[curCity]][curCity]].costTime +
                prevFinTime[prev[curCity]];
        int prevTime = prevFinTime[curCity];
        
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
            
            if(waitTime + prevTime > timeLmt) {
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
                
                prevFinTime[curCity] = prevTime + waitTime;
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
                    << prevFinTime[cityId] 
                    << routeId;
            qDebug()<< "选择路线信息"
                    << route.costTime
                    << route.destCity
                    << route.serialNum
                    << route.startTime
                    << route.vehicleType
                    << "\n";
            
            instance->m_customer->addTask(
                Task(
                    startT,
                    prevFinTime[cityId] - route.costTime,
                    Customer::Waiting,
                    route.serialNum,
                    route.vehicleType,
                    instance->m_absData->m_cityIdHash[cityId].name,
                    instance->m_absData->m_cityIdHash[cityId].name
                )
            );
            instance->m_customer->addTask(
                Task(
                    prevFinTime[cityId] - route.costTime,
                    prevFinTime[cityId],
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
                    prevFinTime[cityId],
                    Customer::Arrive,
                    0,
                    0,
                    instance->m_absData->m_cityIdHash[nextCity].name,
                    instance->m_absData->m_cityIdHash[nextCity].name
                )
            );
            startT = prevFinTime[cityId];
            
            index++;
        }
        
        result += "\n\t总耗时:" +
                    QString::number(prevFinTime[destCityID] / 60) + "小时" +
                    QString::number(prevFinTime[destCityID] % 60) + "分钟";
        result += "\n\t总危险值:" + QString::number(risks[destCityID]);
        
        emit instance->sigNewMessage(
            instance->m_sysTime.toString("[yyyy-MM-dd HH:mm]") +
                result
        );
    }
}
