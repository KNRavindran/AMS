#include "AMSMeterReading.h"

AMSMeterReading::AMSMeterReading(
    const QString& flatNo,
    int readingMonth,
    int readingYear,
    int meterReading,
    QObject *parent)
    : QObject(parent),
    m_flatNo(flatNo),
    m_readingMonth(readingMonth),
    m_readingYear(readingYear),
    m_meterReading(meterReading)
{

}

QString AMSMeterReading::flatNo() const
{
    return m_flatNo;
}

int AMSMeterReading::readingMonth() const
{
    return m_readingMonth;
}

int AMSMeterReading::readingYear() const
{
    return m_readingYear;
}

int AMSMeterReading::meterReading() const
{
    return m_meterReading;
}

void AMSMeterReading::setMeterReading(int meterReading)
{
    if (meterReading < 0)
    {
        return;
    }

    if (m_meterReading == meterReading)
    {
        return;
    }

    m_meterReading = meterReading;

    emit meterReadingChanged();
}