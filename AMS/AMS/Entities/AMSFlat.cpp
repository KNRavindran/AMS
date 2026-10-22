#include "Entities/AMSFlat.h"

AMSFlat::AMSFlat(const QString &flatNo,
                 FlatType flatType,
                 Status status,
                 QObject *parent)
    : QObject(parent),
    m_flatNo(flatNo),
    m_flatType(flatType),
    m_status(status)
{
}

QString AMSFlat::flatNo() const
{
    return m_flatNo;
}

AMSFlat::FlatType AMSFlat::flatType() const
{
    return m_flatType;
}

AMSFlat::Status AMSFlat::status() const
{
    return m_status;
}

void AMSFlat::setFlatType(FlatType flatType)
{
    if (m_flatType == flatType)
    {
        return;
    }

    m_flatType = flatType;

    emit flatTypeChanged();
}

void AMSFlat::setStatus(Status status)
{
    if (m_status == status)
    {
        return;
    }

    m_status = status;

    emit statusChanged();
}

QString AMSFlat::flatTypeToString(FlatType flatType)
{
    switch (flatType)
    {
    case Type1BHK:
        return "1BHK";

    case Type2BHK:
        return "2BHK";

    case Type3BHK:
        return "3BHK";
    }

    return QString();
}

AMSFlat::FlatType
AMSFlat::stringToFlatType(
    const QString& flatType)
{
    if (flatType == "1BHK")
        return Type1BHK;

    if (flatType == "2BHK")
        return Type2BHK;

    if (flatType == "3BHK")
        return Type3BHK;

    // Default

    return Type1BHK;
}

/*
QString AMSFlat::statusToString(Status status)
{
    switch (status)
    {
    case Vacant:
        return "Vacant";

    case Occupied:
        return "Occupied";
    }

    return QString();
}*/

/*
AMSFlat::Status* AMSFlat::stringToStatus(
    const QString& status)
{
    if (status == "Occupied")
        return Occupied;

    return Vacant;
}

*/
int AMSFlat::statusToInt(Status status)
{
    return (status == Occupied) ? 1 : 0;
}

AMSFlat::Status AMSFlat::intToStatus(int value)
{
    return (value == 1)
    ? Occupied
    : Vacant;
}