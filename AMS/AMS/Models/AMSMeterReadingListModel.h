#ifndef AMSMETERREADINGLISTMODEL_H
#define AMSMETERREADINGLISTMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "Entities/AMSMeterReading.h"

class AMSMeterReadingListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    enum MeterReadingRoles
    {
        FlatNoRole = Qt::UserRole + 1,
        ReadingMonthRole,
        ReadingYearRole,
        MeterReadingRole
    };

    explicit AMSMeterReadingListModel(
        QObject *parent = nullptr);

    int rowCount(
        const QModelIndex &parent =
        QModelIndex()) const override;

    QVariant data(
        const QModelIndex &index,
        int role) const override;

    QHash<int, QByteArray> roleNames()
        const override;

    void setMeterReadings(
        const QVector<AMSMeterReading*>& readings);

    AMSMeterReading* meterReadingAt(
        int row) const;
    void refresh();
private:

    QVector<AMSMeterReading*> m_readings;
};

#endif // AMSMETERREADINGLISTMODEL_H