#include "BillSql.h"

QString BillSql::createTable()
{
    return
        "CREATE TABLE IF NOT EXISTS Bill ("
        "FlatNo TEXT NOT NULL,"
        "BillMonth INTEGER NOT NULL "
        "CHECK (BillMonth BETWEEN 1 AND 12),"
        "BillYear INTEGER NOT NULL "
        "CHECK (BillYear >= 2000),"
        "PreviousReading INTEGER NOT NULL "
        "CHECK (PreviousReading >= 0),"
        "CurrentReading INTEGER NOT NULL "
        "CHECK (CurrentReading >= 0),"
        "Units INTEGER NOT NULL "
        "CHECK (Units >= 0),"
        "RatePerUnit REAL NOT NULL "
        "CHECK (RatePerUnit >= 0),"
        "ElectricityAmount REAL NOT NULL "
        "CHECK (ElectricityAmount >= 0),"
        "MaintenanceAmount REAL NOT NULL "
        "CHECK (MaintenanceAmount >= 0),"
        "TotalAmount REAL NOT NULL "
        "CHECK (TotalAmount >= 0),"
        "PRIMARY KEY "
        "(FlatNo, BillMonth, BillYear),"
        "FOREIGN KEY (FlatNo) "
        "REFERENCES Flat(FlatNo)"
        ");";
}

QString BillSql::selectAll()
{
    return
        "SELECT "
        "FlatNo, "
        "BillMonth, "
        "BillYear, "
        "PreviousReading, "
        "CurrentReading, "
        "Units, "
        "RatePerUnit, "
        "ElectricityAmount, "
        "MaintenanceAmount, "
        "TotalAmount "
        "FROM Bill "
        "ORDER BY BillYear DESC, "
        "BillMonth DESC, "
        "FlatNo;";
}

QString BillSql::insert()
{
    return
        "INSERT INTO Bill ("
        "FlatNo, "
        "BillMonth, "
        "BillYear, "
        "PreviousReading, "
        "CurrentReading, "
        "Units, "
        "RatePerUnit, "
        "ElectricityAmount, "
        "MaintenanceAmount, "
        "TotalAmount"
        ") "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
}

QString BillSql::update()
{
    return
        "UPDATE Bill "
        "SET "
        "PreviousReading = ?, "
        "CurrentReading = ?, "
        "Units = ?, "
        "RatePerUnit = ?, "
        "ElectricityAmount = ?, "
        "MaintenanceAmount = ?, "
        "TotalAmount = ? "
        "WHERE FlatNo = ? "
        "AND BillMonth = ? "
        "AND BillYear = ?;";
}

QString BillSql::remove()
{
    return
        "DELETE FROM Bill "
        "WHERE FlatNo = ? "
        "AND BillMonth = ? "
        "AND BillYear = ?;";
}