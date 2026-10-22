#include "Sql/FlatSql.h"
#include <QDebug>
QString FlatSql::selectAll()
{
    return
        "SELECT "
        "FlatNo, "
        "FlatType, "
        "Status "
        "FROM Flat";
}

QString FlatSql::insert()
{
    return
        "INSERT INTO Flat "
        "("
        "FlatNo,"
        "FlatType,"
        "Status"
        ") "
        "VALUES "
        "("
        ":flatNo,"
        ":flatType,"
        ":status"
        ")";
}

QString FlatSql::update()
{
    return
        "UPDATE Flat "
        "SET "
        "FlatType = :flatType, "
        "Status = :status "
        "WHERE "
        "FlatNo = :flatNo";
}


QString FlatSql::exists()
{
    return "";
}

QString FlatSql::remove()
{
    return
        "DELETE FROM Flat "
        "WHERE "
        "FlatNo = :flatNo";
}