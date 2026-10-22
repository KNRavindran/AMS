#include "AMSBill.h"
#include <QDebug>
AMSBill::AMSBill(
    const QString& flatNo,
    int billMonth,
    int billYear,
    int previousReading,
    int currentReading,
    int unitsConsumed,
    double ratePerUnit,
    double electricityAmount,
    double maintenanceAmount,
    double totalAmount,
    QObject *parent)
    : QObject(parent),
    m_flatNo(flatNo),
    m_billMonth(billMonth),
    m_billYear(billYear),
    m_previousReading(previousReading),
    m_currentReading(currentReading),
    m_unitsConsumed(unitsConsumed),
    m_ratePerUnit(ratePerUnit),
    m_electricityAmount(electricityAmount),
    m_maintenanceAmount(maintenanceAmount),
    m_totalAmount(totalAmount)
{
}

QString AMSBill::flatNo() const
{
    return m_flatNo;
}

int AMSBill::billMonth() const
{
    return m_billMonth;
}

int AMSBill::billYear() const
{
    return m_billYear;
}

int AMSBill::previousReading() const
{
    return m_previousReading;
}

int AMSBill::currentReading() const
{
    return m_currentReading;
}

int AMSBill::unitsConsumed() const
{
    return m_unitsConsumed;
}

double AMSBill::ratePerUnit() const
{
    return m_ratePerUnit;
}

double AMSBill::electricityAmount() const
{
    return m_electricityAmount;
}

double AMSBill::maintenanceAmount() const
{
    return m_maintenanceAmount;
}

double AMSBill::totalAmount() const
{
    return m_totalAmount;
}



void AMSBill::setSelected(bool value)
{
    m_selected = value;
}

bool AMSBill::selected() const
{
    return m_selected;
}
