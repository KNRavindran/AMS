#ifndef AMSMETERREADINGMANAGER_H
#define AMSMETERREADINGMANAGER_H

#include <QObject>
#include <QList>

#include "Entities/AMSMeterReading.h"
#include "Models/AMSMeterReadingListModel.h"

class AMSMeterReadingManager : public QObject
{
    Q_OBJECT

public:

    explicit AMSMeterReadingManager(
        QObject *parent = nullptr);

    bool loadMeterReadings();

    bool addMeterReading(
        const QString& flatNo,
        int readingMonth,
        int readingYear,
        int meterReading);

    bool updateMeterReading(
        const QString& flatNo,
        int readingMonth,
        int readingYear,
        int meterReading);

    bool deleteMeterReading(
        const QString& flatNo,
        int readingMonth,
        int readingYear);

    bool meterReadingExists(
        const QString& flatNo,
        int readingMonth,
        int readingYear) const;

    AMSMeterReading* findMeterReading(
        const QString& flatNo,
        int readingMonth,
        int readingYear) const;

    AMSMeterReading* previousReading(
        const QString& flatNo,
        int currentMonth,
        int currentYear) const;
    QList<AMSMeterReading*> meterReadings() const;

    AMSMeterReadingListModel* meterReadingListModel() const;
private:

    QList<AMSMeterReading*> m_meterReadings;
    AMSMeterReadingListModel* m_meterReadingListModel;
};

#endif // AMSMETERREADINGMANAGER_H