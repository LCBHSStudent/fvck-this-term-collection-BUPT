#ifndef CITYMODEL_H
#define CITYMODEL_H

#include <QObject>
#include <cstdint>
#include <QAbstractListModel>

class CityData;

class CityModel: public QAbstractListModel {
    Q_OBJECT
public:
    explicit
        CityModel(QObject *parent = nullptr);
    virtual
        ~CityModel();
    
    enum DataRoles {
        CityIDRole = Qt::UserRole + 1,
        CityNameRole,
        CityDangerFactorRole
    };    
    
    int
        rowCount(
            const QModelIndex& parent = QModelIndex() 
        ) const override;
    QVariant
        data(
            const QModelIndex& index, int role = Qt::DisplayRole
        ) const override;
    void
        append(
            const int64_t   CityID,
            const QString&  CityName,
            const float     CityDangerFactor
        );
    void
        insert(int index, const CityData& data);
    
protected:
    virtual QHash<int, QByteArray>
        roleNames() const override;
        
private:
    QList<CityData> m_dataList;
    
};

#endif // CityMODEL_H
