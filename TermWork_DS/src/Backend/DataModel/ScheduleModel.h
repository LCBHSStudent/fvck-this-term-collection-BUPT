#ifndef SCHEDULEMODEL_H
#define SCHEDULEMODEL_H

#include <cstdint>
#include <QAbstractListModel>

class ScheduleData;

class ScheduleModel: public QAbstractListModel {
    Q_OBJECT
public:
    explicit
        ScheduleModel(QObject *parent = nullptr);
    virtual
        ~ScheduleModel();
    
    enum DataRoles {
        TACNumberRole = Qt::UserRole + 1,
        FromStationRole,
        ToStationRole,
        StartHRole,
        StartMRole,
        ArriveHRole,
        ArriveMRole,
        TransportationTypeRole
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
            const int       TACNumber,
            const QString&  FromStation,
            const QString&  ToStation,
            const int       StartH,
            const int       StartM,
            const int       ArriveH,
            const int       ArriveM,
            const int       TransportationType
        );
    void
        insert(int index, const ScheduleData& data);
    
protected:
    virtual QHash<int, QByteArray>
        roleNames() const override;
        
private:
    QList<ScheduleData> m_dataList;
    
};

#endif // SCHEDULEMODEL_H
