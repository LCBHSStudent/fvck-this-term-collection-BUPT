#ifndef ABSTRACTDATA_H
#define ABSTRACTDATA_H

#include <vector>

#include "ScheduleModel.h"
#include "CityModel.h"

#define RESOURCE 

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
    int     id      = -1;
    float   factor  = 0.0;
    QString name    = {};
};

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
    int     vehicleType;
    int     destCity;
    int     startTime;
    int     costTime;
    int     serialNum;
};


class AbstractData final {
    friend class BackendBase;
    friend class AlgorithmHelper;
public:
     AbstractData() = default;
    ~AbstractData() = default;

    void
        printAllRouteInfo(QString& cityName);
    
private:
     std::vector<
        std::vector<AbstractRoute>
     >
        m_routeMap {};
     QHash<QString, AbstractCity>
        m_cityHash {};
     QHash<int, AbstractCity>
        m_cityIdHash {};
};

#endif // ABSTRACTDATA_H
