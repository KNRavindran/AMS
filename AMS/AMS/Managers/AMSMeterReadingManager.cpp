#include "AMSMeterReadingManager.h"
#include "Sql/MeterReadingSql.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>

AMSMeterReadingManager::AMSMeterReadingManager(
    QObject *parent)
    : QObject(parent)
{
    m_meterReadingListModel =
        new AMSMeterReadingListModel(this);

    loadMeterReadings();
}

QList<AMSMeterReading*> AMSMeterReadingManager::meterReadings() const
{
    return m_meterReadings;
}

bool AMSMeterReadingManager::meterReadingExists(
    const QString& flatNo,
    int readingMonth,
    int readingYear) const
{
    return findMeterReading(
               flatNo,
               readingMonth,
               readingYear) != nullptr;
}

AMSMeterReading*
AMSMeterReadingManager::findMeterReading(
    const QString& flatNo,
    int readingMonth,
    int readingYear) const
{
    for (AMSMeterReading* meterReading : m_meterReadings)
    {
        if (meterReading->flatNo() == flatNo &&
            meterReading->readingMonth() == readingMonth &&
            meterReading->readingYear() == readingYear)
        {
            return meterReading;
        }
    }

    return nullptr;
}


bool AMSMeterReadingManager::addMeterReading(
    const QString& flatNo,
    int readingMonth,
    int readingYear,
    int meterReading)
{
    QString trimmedFlatNo = flatNo.trimmed();

    if (trimmedFlatNo.isEmpty())
        return false;

    if (readingMonth < 1 || readingMonth > 12)
        return false;

    if (readingYear < 2000)
        return false;

    if (meterReading < 0)
        return false;

    if (meterReadingExists(
            trimmedFlatNo,
            readingMonth,
            readingYear))
    {
        qDebug() << "Duplicate found.";

        return false;
    }

    QSqlQuery query(QSqlDatabase::database());

    query.prepare(
        MeterReadingSql::insert());

    query.addBindValue(trimmedFlatNo);
    query.addBindValue(readingMonth);
    query.addBindValue(readingYear);
    query.addBindValue(meterReading);

    if (!query.exec())
    {
        qDebug() << "Insert MeterReading Failed:"
                 << query.lastError().text();

        return false;
    }

    qDebug() << "Meter reading inserted.";

    loadMeterReadings();

    return true;
}

bool AMSMeterReadingManager::loadMeterReadings()
{
    qDeleteAll(m_meterReadings);

    m_meterReadings.clear();

    QSqlQuery query(QSqlDatabase::database());

    if (!query.exec(MeterReadingSql::selectAll()))
    {
        qDebug() << "Load Meter Readings Failed:"
                 << query.lastError().text();

        return false;
    }

    QSqlRecord record = query.record();

    int flatNoIndex =
        record.indexOf("FlatNo");

    int monthIndex =
        record.indexOf("ReadingMonth");

    int yearIndex =
        record.indexOf("ReadingYear");

    int readingIndex =
        record.indexOf("MeterReading");

    while (query.next())
    {
        QString flatNo =
            query.value(flatNoIndex).toString();

        int month =
            query.value(monthIndex).toInt();

        int year =
            query.value(yearIndex).toInt();

        int meterReading =
            query.value(readingIndex).toInt();

        AMSMeterReading *reading =
            new AMSMeterReading(
                flatNo,
                month,
                year,
                meterReading,
                this);

        m_meterReadings.append(reading);
    }

    m_meterReadingListModel->setMeterReadings(
        QVector<AMSMeterReading*>::fromList(m_meterReadings));

    qDebug() << "Loaded Meter Readings:"
             << m_meterReadings.count();

    return true;
}

bool AMSMeterReadingManager::updateMeterReading(
    const QString& flatNo,
    int readingMonth,
    int readingYear,
    int meterReading)
{
    QString trimmedFlatNo = flatNo.trimmed();

    if (trimmedFlatNo.isEmpty())
        return false;

    if (meterReading < 0)
        return false;

    AMSMeterReading *reading =
        findMeterReading(
            trimmedFlatNo,
            readingMonth,
            readingYear);

    if (!reading)
        return false;

    QSqlQuery query(QSqlDatabase::database());

    query.prepare(
        MeterReadingSql::update());

    query.addBindValue(meterReading);
    query.addBindValue(trimmedFlatNo);
    query.addBindValue(readingMonth);
    query.addBindValue(readingYear);

    if (!query.exec())
    {
        qDebug() << "Update MeterReading Failed:"
                 << query.lastError().text();

        return false;
    }

    loadMeterReadings();

    return true;
}

bool AMSMeterReadingManager::deleteMeterReading(
    const QString& flatNo,
    int readingMonth,
    int readingYear)
{
    QString trimmedFlatNo = flatNo.trimmed();

    if (trimmedFlatNo.isEmpty())
        return false;

    AMSMeterReading *reading =
        findMeterReading(
            trimmedFlatNo,
            readingMonth,
            readingYear);

    if (!reading)
        return false;

    QSqlQuery query(QSqlDatabase::database());

    query.prepare(
        MeterReadingSql::remove());

    query.addBindValue(trimmedFlatNo);
    query.addBindValue(readingMonth);
    query.addBindValue(readingYear);

    if (!query.exec())
    {
        qDebug() << "Delete MeterReading Failed:"
                 << query.lastError().text();

        return false;
    }

    loadMeterReadings();

    return true;
}

AMSMeterReading* AMSMeterReadingManager::previousReading(
    const QString& flatNo,
    int currentMonth,
    int currentYear) const
{
    int previousMonth;
    int previousYear;

    if (currentMonth == 1)
    {
        previousMonth = 12;
        previousYear = currentYear - 1;
    }
    else
    {
        previousMonth = currentMonth - 1;
        previousYear = currentYear;
    }

    return findMeterReading(
        flatNo,
        previousMonth,
        previousYear);
}

AMSMeterReadingListModel*
AMSMeterReadingManager::meterReadingListModel() const
{
    return m_meterReadingListModel;
}