#include "AbstractData.h"

#include <QDebug>

void AbstractData::printAllRouteInfo(QString& cityName) {
    for(auto& route: m_routeMap[m_cityHash[cityName].id]) {
        qDebug() << "[Destination]: "  << route.destCity 
                 << "\n[Start time]: " << route.startTime
                 << "\n[Cost time]: "  << route.costTime
                 << "\n[Vehicle type]: " << route.vehicleType << "\n\n";
    }
}
