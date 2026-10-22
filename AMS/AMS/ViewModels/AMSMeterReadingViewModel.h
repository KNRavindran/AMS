#ifndef AMSMETERREADINGVIEWMODEL_H
#define AMSMETERREADINGVIEWMODEL_H

#include <QObject>
#include "Models/AMSMeterReadingListModel.h"

class AMSMeterReadingManager;

class AMSMeterReadingViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(
        AMSMeterReadingListModel* meterReadingModel
            READ meterReadingModel
                CONSTANT)

    Q_PROPERTY(
        QString selectedFlatNo
            READ selectedFlatNo
                NOTIFY selectedMeterReadingChanged)

    Q_PROPERTY(
        int selectedMonth
            READ selectedMonth
                NOTIFY selectedMeterReadingChanged)

    Q_PROPERTY(
        int selectedYear
            READ selectedYear
                NOTIFY selectedMeterReadingChanged)

    Q_PROPERTY(
        int selectedReading
            READ selectedReading
                NOTIFY selectedMeterReadingChanged)

    Q_PROPERTY(
        int selectedRow
            READ selectedRow
                NOTIFY selectedMeterReadingChanged)
    Q_PROPERTY(
        int meterReadingCount
            READ meterReadingCount
                NOTIFY meterReadingCountChanged)

public:

    explicit AMSMeterReadingViewModel(
        AMSMeterReadingManager *meterReadingManager,
        QObject *parent = nullptr);

    Q_INVOKABLE bool addMeterReading(
        const QString& flatNo,
        int month,
        int year,
        int reading);

    Q_INVOKABLE bool updateMeterReading(
        const QString& flatNo,
        int month,
        int year,
        int reading);

    Q_INVOKABLE bool deleteMeterReading(
        const QString& flatNo,
        int month,
        int year);

    Q_INVOKABLE QStringList meterReadingList() const;

    Q_INVOKABLE void selectMeterReading(
        int index);

    Q_INVOKABLE void clearSelection();

    AMSMeterReadingListModel* meterReadingModel() const;

    QString selectedFlatNo() const;

    int selectedMonth() const;

    int selectedYear() const;

    int selectedReading() const;

    int selectedRow() const;
    int meterReadingCount() const;

signals:

    void operationCompleted(
        bool success,
        QString message);

    void selectedMeterReadingChanged();
    void meterReadingCountChanged();
private:

    AMSMeterReadingManager *m_meterReadingManager;

    QString m_selectedFlatNo;

    int m_selectedMonth;

    int m_selectedYear;

    int m_selectedReading;

    int m_selectedRow;
};

#endif // AMSMETERREADINGVIEWMODEL_H