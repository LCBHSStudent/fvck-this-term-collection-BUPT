#include "CityModel.h"

class CityData {
public:
    CityData(
        const int64_t   CityID,
        const QString&  CityName,
        const float     CityDangerFactor
    );
    
    int64_t CityID() const;
    QString CityName() const;
    float   CityDangerFactor() const;
    
private:
    QString 
        m_cityName;
    int64_t
        m_cityId;
    float
        m_cityDangerFactor;
};

CityData::CityData(
    const int64_t   CityID,
    const QString&  CityName,
    const float     CityDangerFactor
):
    m_cityName(CityName),
    m_cityId(CityID),
    m_cityDangerFactor(CityDangerFactor) {}

int64_t CityData::CityID() const {
    return m_cityId;
}

QString CityData::CityName() const {
    return m_cityName;
}

float CityData::CityDangerFactor() const {
    return m_cityDangerFactor;
}

//---------------------------CityModel------------------------------//

CityModel::CityModel(QObject *parent):
    QAbstractListModel(parent) {}

CityModel::~CityModel() {}

int CityModel::rowCount(const QModelIndex &parent) const {
    (void)parent;
    return m_dataList.count();
}

QVariant CityModel::data(const QModelIndex &index, int role) const {
    int row = index.row();
    if (row < 0 || row > m_dataList.count()) {
        return QVariant();
    }
    
    const CityData &data = m_dataList[row];
    switch (role) {
    case CityIDRole:
        return data.CityID();
    case CityNameRole:
        return data.CityName();
    case CityDangerFactorRole:
        return data.CityDangerFactor();
    default:
        return QVariant();
    }
}

void CityModel::insert(int index, const CityData &data) {
    if(index < 0 || index > m_dataList.count()) {
        return;
    }
    
    emit beginInsertRows(QModelIndex(), index, index);
    m_dataList.insert(index, data);
    emit endInsertRows();
}

void CityModel::append(
    const int64_t   CityID,
    const QString   &CityName,
    const float     CityDangerFactor
) {
    insert(
        rowCount(QModelIndex()),
        CityData(CityID, CityName, CityDangerFactor)
    );
}

QHash<int, QByteArray> CityModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[CityIDRole]           = "CityID";
    roles[CityNameRole]         = "CityName";
    roles[CityDangerFactorRole] = "CityDangerFactor";
    return roles;
}
