#include "AMSMeterReadingViewModel.h"

#include "Managers/AMSMeterReadingManager.h"
#include "Entities/AMSMeterReading.h"

AMSMeterReadingViewModel::AMSMeterReadingViewModel(
    AMSMeterReadingManager *meterReadingManager,
    QObject *parent)
    : QObject(parent),
    m_selectedMonth(1),
    m_selectedYear(2026),
    m_selectedReading(0),
    m_selectedRow(-1),
    m_meterReadingManager(meterReadingManager)
{
}

AMSMeterReadingListModel*
AMSMeterReadingViewModel::meterReadingModel() const
{
    return m_meterReadingManager->meterReadingListModel();
}

bool AMSMeterReadingViewModel::addMeterReading(
    const QString &flatNo,
    int month,
    int year,
    int reading)
{
    bool success =
        m_meterReadingManager->addMeterReading(
            flatNo,
            month,
            year,
            reading);

    emit operationCompleted(
        success,
        success ?
            "Meter reading added successfully."
                : "Failed to add meter reading.");
    if(success)
    {
        emit meterReadingCountChanged();
    }
    return success;
}

bool AMSMeterReadingViewModel::updateMeterReading(
    const QString &flatNo,
    int month,
    int year,
    int reading)
{
    bool success =
        m_meterReadingManager->updateMeterReading(
            flatNo,
            month,
            year,
            reading);

    emit operationCompleted(
        success,
        success ?
            "Meter reading updated successfully."
                : "Failed to update meter reading.");
    if(success)
    {
        emit meterReadingCountChanged();
    }

    return success;
}

bool AMSMeterReadingViewModel::deleteMeterReading(
    const QString &flatNo,
    int month,
    int year)
{
    bool success =
        m_meterReadingManager->deleteMeterReading(
            flatNo,
            month,
            year);

    emit operationCompleted(
        success,
        success ?
            "Meter reading deleted successfully."
                : "Failed to delete meter reading.");

    if(success)
    {
        emit meterReadingCountChanged();
    }

    return success;
}

QStringList AMSMeterReadingViewModel::meterReadingList() const
{
    QStringList list;

    QList<AMSMeterReading*> readings =
        m_meterReadingManager->meterReadings();

    for (AMSMeterReading *reading : readings)
    {
        QString row =
            reading->flatNo()
            + ","
            + QString::number(reading->readingMonth())
            + ","
            + QString::number(reading->readingYear())
            + ","
            + QString::number(reading->meterReading());

        list.append(row);
    }

    return list;
}

QString AMSMeterReadingViewModel::selectedFlatNo() const
{
    return m_selectedFlatNo;
}

int AMSMeterReadingViewModel::selectedMonth() const
{
    return m_selectedMonth;
}

int AMSMeterReadingViewModel::selectedYear() const
{
    return m_selectedYear;
}

int AMSMeterReadingViewModel::selectedReading() const
{
    return m_selectedReading;
}

int AMSMeterReadingViewModel::selectedRow() const
{
    return m_selectedRow;
}

void AMSMeterReadingViewModel::selectMeterReading(
    int index)
{
    m_selectedRow = index;

    AMSMeterReading *reading =
        m_meterReadingManager
            ->meterReadingListModel()
            ->meterReadingAt(index);

    if (!reading)
    {
        return;
    }

    m_selectedFlatNo = reading->flatNo();
    m_selectedMonth = reading->readingMonth();
    m_selectedYear = reading->readingYear();
    m_selectedReading = reading->meterReading();

    emit selectedMeterReadingChanged();

    m_meterReadingManager
        ->meterReadingListModel()
        ->refresh();
}

void AMSMeterReadingViewModel::clearSelection()
{
    m_selectedRow = -1;

    m_selectedFlatNo.clear();

    m_selectedMonth = 1;
    m_selectedYear = 2026;
    m_selectedReading = 0;

    emit selectedMeterReadingChanged();

    m_meterReadingManager
        ->meterReadingListModel()
        ->refresh();
}

int AMSMeterReadingViewModel::meterReadingCount() const
{
    return
        m_meterReadingManager
            ->meterReadings()
            .count();
}