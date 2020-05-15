#include "ScheduleModel.h"

class ScheduleData {
public:
    ScheduleData(
        const int       TACNumber,
        const QString&  FromStation,
        const QString&  ToStation,
        const int       StartH,
        const int       StartM,
        const int       ArriveH,
        const int       ArriveM,
        const int       TransportationType
    );
    
    int 
        TACNumber() const { return m_tacNumber; }
    QString
        FromStation() const { return m_fromStation; }
    QString 
        ToStation() const { return m_toStation; }
    int       
        StartH() const { return m_startH; }
    int       
        StartM() const { return m_startM; }
    int       
        ArriveH() const { return m_arriveH; }
    int       
        ArriveM() const { return m_arriveM; }
    int       
        TransportationType() const { return m_transportationType; }
        
private:
    int 
        m_tacNumber;
    QString
        m_fromStation;
    QString
        m_toStation;
    int       
        m_startH;
    int       
        m_startM;
    int       
        m_arriveH;
    int       
        m_arriveM;
    int       
        m_transportationType;
};

ScheduleData::ScheduleData(
    const int       TACNumber,
    const QString&  FromStation,
    const QString&  ToStation,
    const int       StartH,
    const int       StartM,
    const int       ArriveH,
    const int       ArriveM,
    const int       TransportationType    
):
    m_tacNumber(TACNumber),
    m_fromStation(FromStation),
    m_toStation(ToStation),
    m_startH(StartH),
    m_startM(StartM),
    m_arriveH(ArriveH),
    m_arriveM(ArriveM),
    m_transportationType(TransportationType) {}

//-----------------------------ScheudleModel------------------------------//

ScheduleModel::ScheduleModel(QObject *parent):
    QAbstractListModel(parent) {}

ScheduleModel::~ScheduleModel() {}

int ScheduleModel::rowCount(const QModelIndex &parent) const {
    (void)parent;
    return m_dataList.count();
}

QVariant ScheduleModel::data(const QModelIndex &index, int role) const {
    int row = index.row();
    if (row < 0 || row > m_dataList.count()) {
        return QVariant();
    }
    
    const ScheduleData &data = m_dataList[row];
    switch (role) {
    case TACNumberRole:
        return data.TACNumber();
    case FromStationRole:
        return data.FromStation();
    case ToStationRole:
        return data.ToStation();
    case StartHRole:
        return data.StartH();
    case StartMRole:
        return data.StartM();
    case ArriveHRole:
        return data.ArriveH();
    case ArriveMRole:
        return data.ArriveM();
    case TransportationTypeRole:
        return data.TransportationType();
    default:
        return QVariant();
    }
}

void ScheduleModel::insert(int index, const ScheduleData &data) {
    if(index < 0 || index > m_dataList.count()) {
        return;
    }
    
    emit beginInsertRows(QModelIndex(), index, index);
    m_dataList.insert(index, data);
    emit endInsertRows();
}

void ScheduleModel::append(
    const int       TACNumber,
    const QString&  FromStation,
    const QString&  ToStation,
    const int       StartH,
    const int       StartM,
    const int       ArriveH,
    const int       ArriveM,
    const int       TransportationType    
) {
    insert(
        rowCount(QModelIndex()),
        ScheduleData(
            TACNumber,
            FromStation, ToStation,
            StartH,      StartM,
            ArriveH,     ArriveM,
            TransportationType)
    );
}

QHash<int, QByteArray> ScheduleModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TACNumberRole]            = "TACNumber";
    roles[FromStationRole]          = "FromStation";
    roles[ToStationRole]            = "ToStation";
    roles[StartHRole]               = "StartH";
    roles[StartMRole]               = "StartM";
    roles[ArriveHRole]              = "ArriveH";
    roles[ArriveMRole]              = "ArriveM";
    roles[TransportationTypeRole] = "TransportationType";
    return roles;
}
