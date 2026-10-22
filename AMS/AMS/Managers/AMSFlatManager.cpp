#include "Managers/AMSFlatManager.h"

#include "Database/AMSDatabase.h"
#include "Entities/AMSFlat.h"
#include "Sql/FlatSql.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

AMSFlatManager::AMSFlatManager(
    AMSDatabase *database,
    QObject *parent)
    : QObject(parent),
    m_database(database)
{
    if (m_database == nullptr)
    {
        qFatal("AMSFlatManager : Database pointer is NULL.");
    }

    loadFlats();
}

AMSFlatManager::~AMSFlatManager()
{
    qDeleteAll(m_flats);

    m_flats.clear();
}

bool AMSFlatManager::addFlat(
    const QString &flatNo,
    AMSFlat::FlatType flatType,
    AMSFlat::Status status)
{
    QString trimmedFlatNo = flatNo.trimmed();

    if (trimmedFlatNo.isEmpty())
    {
        if (!isValidFlatNo(
                flatNo,
                trimmedFlatNo))
        {
            return false;
        }
    }
    if (flatExists(trimmedFlatNo))
    {
        qDebug() << "Flat already exists.";

        return false;
    }
    QSqlQuery query(m_database->database());

    query.prepare(FlatSql::insert());
    query.bindValue(
        ":flatNo",
        trimmedFlatNo);

    query.bindValue(
        ":flatType",
        AMSFlat::flatTypeToString(flatType));

    query.bindValue(
        ":status",
        AMSFlat::statusToInt(status));

    if (!m_database->executeNonQuery(query))
    {
        return false;
    }
    AMSFlat *flat =
        new AMSFlat(
            trimmedFlatNo,
            flatType,
            status);

    m_flats.append(flat);
    m_flatListModel.setFlats(m_flats);
    qDebug() << "Flat added :" << trimmedFlatNo;

    return true;
    //return false;
}

bool AMSFlatManager::updateFlat(
    const QString &flatNo,
    AMSFlat::FlatType flatType,
    AMSFlat::Status status)
{
    QString trimmedFlatNo;

    if (!isValidFlatNo(flatNo, trimmedFlatNo))
    {
        return false;
    }

    AMSFlat *flat = findFlat(trimmedFlatNo);

    if (flat == nullptr)
    {
        qDebug() << "Flat not found.";

        return false;
    }

    QSqlQuery query(m_database->database());

    query.prepare(
        FlatSql::update());

    query.bindValue(
        ":flatNo",
        trimmedFlatNo);

    query.bindValue(
        ":flatType",
        AMSFlat::flatTypeToString(flatType));

    query.bindValue(
        ":status",
        AMSFlat::statusToInt(status));

    if (!m_database->executeNonQuery(query))
    {
        return false;
    }

    flat->setFlatType(flatType);
    flat->setStatus(status);

    m_flatListModel.setFlats(m_flats);
    qDebug() << "Flat updated:" << trimmedFlatNo;

    return true;
}

bool AMSFlatManager::deleteFlat(
    const QString &flatNo)
{
    QString trimmedFlatNo;

    if (!isValidFlatNo(flatNo, trimmedFlatNo))
    {
        return false;
    }

    AMSFlat *flat = findFlat(trimmedFlatNo);

    if (flat == nullptr)
    {
        qDebug() << "Flat not found.";

        return false;
    }

    QSqlQuery query(m_database->database());

    query.prepare(
        FlatSql::remove());

    query.bindValue(
        ":flatNo",
        trimmedFlatNo);

    if (!m_database->executeNonQuery(query))
    {
        return false;
    }

    m_flats.removeOne(flat);

    delete flat;
    m_flatListModel.setFlats(m_flats);

    qDebug() << "Flat deleted:" << trimmedFlatNo;

    return true;
}

bool AMSFlatManager::flatExists(
    const QString &flatNo) const
{
    return findFlat(flatNo) != nullptr;
}

AMSFlat *AMSFlatManager::findFlat(
    const QString &flatNo) const
{
    QString trimmedFlatNo = flatNo.trimmed();

    for (AMSFlat *flat : m_flats)
    {
        if (flat->flatNo() == trimmedFlatNo)
        {
            return flat;
        }
    }

    return nullptr;
}

bool AMSFlatManager::loadFlats()
{
    qDeleteAll(m_flats);
    m_flats.clear();
    /*
    QString sql =
        "SELECT "
        "FlatNo, "
        "FlatType, "
        "Status "
        "FROM Flat";
    */
    /*
    QString sql = FlatSql::selectAll();
    QSqlQuery query = m_database->executeQuery(sql);
    */

    QSqlQuery query =
        m_database->executeQuery(
            FlatSql::selectAll());
    if (query.lastError().isValid())
    {
        return false;
    }

    QSqlRecord record = query.record();

    int flatNoIndex   = record.indexOf("FlatNo");
    int flatTypeIndex = record.indexOf("FlatType");
    int statusIndex   = record.indexOf("Status");

    while (query.next())
    {
        QString flatNo =
            query.value(flatNoIndex).toString();

        QString flatTypeText =
            query.value(flatTypeIndex).toString();

        int statusValue =
            query.value(statusIndex).toInt();

        AMSFlat::FlatType flatType =
            AMSFlat::stringToFlatType(flatTypeText);


        // AMSFlat::Status status =
        //     (statusValue == 0)
        //         ? AMSFlat::Vacant
        //         : AMSFlat::Occupied;
        AMSFlat::Status status =
            AMSFlat::intToStatus(statusValue);

        AMSFlat *flat =
            new AMSFlat(
                flatNo,
                flatType,
                status);

        m_flats.append(flat);
        qDebug() << "Loaded Flat :" << flatNo;
        m_flatListModel.setFlats(m_flats);
    }

    return true;
}

bool AMSFlatManager::isValidFlatNo(
    const QString &flatNo,
    QString &trimmedFlatNo) const
{
    trimmedFlatNo = flatNo.trimmed();

    if (trimmedFlatNo.isEmpty())
    {
        qDebug() << "Flat Number cannot be empty.";

        return false;
    }

    return true;
}

const QList<AMSFlat*>& AMSFlatManager::flats() const
{
    return m_flats;
}

AMSFlatListModel* AMSFlatManager::flatListModel()
{
    return &m_flatListModel;
}