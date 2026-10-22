#include "MeterReadingSql.h"

QString MeterReadingSql::createTable()
{
    return
        "CREATE TABLE IF NOT EXISTS MeterReading ("
        "FlatNo TEXT NOT NULL,"
        "ReadingMonth INTEGER NOT NULL "
        "CHECK (ReadingMonth BETWEEN 1 AND 12),"
        "ReadingYear INTEGER NOT NULL "
        "CHECK (ReadingYear >= 2000),"
        "MeterReading INTEGER NOT NULL "
        "CHECK (MeterReading >= 0),"
        "PRIMARY KEY "
        "(FlatNo, ReadingMonth, ReadingYear),"
        "FOREIGN KEY (FlatNo) "
        "REFERENCES Flat(FlatNo)"
        ");";
}


QString MeterReadingSql::selectAll()
{
    return
        "SELECT "
        "FlatNo, "
        "ReadingMonth, "
        "ReadingYear, "
        "MeterReading "
        "FROM MeterReading;";
}

QString MeterReadingSql::insert()
{
    return
        "INSERT INTO MeterReading ("
        "FlatNo, "
        "ReadingMonth, "
        "ReadingYear, "
        "MeterReading"
        ") "
        "VALUES (?, ?, ?, ?);";
}

QString MeterReadingSql::update()
{
    return
        "UPDATE MeterReading "
        "SET MeterReading = ? "
        "WHERE FlatNo = ? "
        "AND ReadingMonth = ? "
        "AND ReadingYear = ?;";
}

QString MeterReadingSql::remove()
{
    return
        "DELETE FROM MeterReading "
        "WHERE FlatNo = ? "
        "AND ReadingMonth = ? "
        "AND ReadingYear = ?;";
}