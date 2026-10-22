#include "AMSMeterReadingListModel.h"

AMSMeterReadingListModel::AMSMeterReadingListModel(
    QObject *parent)
    : QAbstractListModel(parent)
{
}

int AMSMeterReadingListModel::rowCount(
    const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_readings.count();
}

QVariant AMSMeterReadingListModel::data(
    const QModelIndex &index,
    int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.row() < 0 ||
        index.row() >= m_readings.count())
    {
        return QVariant();
    }

    AMSMeterReading *reading =
        m_readings.at(index.row());

    switch (role)
    {
    case FlatNoRole:
        return reading->flatNo();

    case ReadingMonthRole:
        return reading->readingMonth();

    case ReadingYearRole:
        return reading->readingYear();

    case MeterReadingRole:
        return reading->meterReading();
    }

    return QVariant();
}

QHash<int, QByteArray>
AMSMeterReadingListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[FlatNoRole] = "flatNo";
    roles[ReadingMonthRole] = "readingMonth";
    roles[ReadingYearRole] = "readingYear";
    roles[MeterReadingRole] = "meterReading";

    return roles;
}

void AMSMeterReadingListModel::setMeterReadings(
    const QVector<AMSMeterReading *> &readings)
{
    beginResetModel();

    m_readings = readings;

    endResetModel();
}

AMSMeterReading *
AMSMeterReadingListModel::meterReadingAt(
    int row) const
{
    if (row < 0 ||
        row >= m_readings.count())
    {
        return nullptr;
    }

    return m_readings.at(row);
}

void AMSMeterReadingListModel::refresh()
{
    if (m_readings.isEmpty())
    {
        return;
    }

    emit dataChanged(
        index(0, 0),
        index(rowCount() - 1, 0));
}