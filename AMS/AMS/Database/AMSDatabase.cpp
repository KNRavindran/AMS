#include "AMSDatabase.h"
#include <QtSql/QSqlError>
#include "Sql/FlatSql.h"
#include "Sql/MeterReadingSql.h"
#include "Sql/BillSql.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>

AMSDatabase::AMSDatabase(QObject *parent)
    : QObject(parent)
{
}

AMSDatabase::~AMSDatabase()
{
    close();
}

bool AMSDatabase::open()
{
    if (m_db.isOpen())

    {
        QSqlQuery query(m_db);

        query.exec("PRAGMA foreign_keys = ON;");
        return true;
    }
    QString dataFolder =
        QCoreApplication::applicationDirPath() + "/Data";

    QDir dir;

    if (!dir.exists(dataFolder))
    {
        dir.mkpath(dataFolder);
    }

    QString dbFile = dataFolder + "/AMS.db";

    qDebug() << "Database File :" << dbFile;

    m_db = QSqlDatabase::addDatabase("QSQLITE");

    m_db.setDatabaseName(dbFile);

    if (!m_db.open())
    {
        m_lastError = m_db.lastError().text();

        qDebug() << "Database Open Failed:" << m_lastError;

        return false;
    }

    qDebug() << "Database Opened Successfully.";

    if (!createTables())
    {
        return false;
    }

    return true;
}


bool AMSDatabase::createTables()
{
    QSqlQuery query(m_db);

    QString flatSql =
        "CREATE TABLE IF NOT EXISTS Flat ("
        "FlatNo TEXT PRIMARY KEY,"
        "FlatType TEXT NOT NULL,"
        "Status TEXT NOT NULL"
        ");";

    if (!query.exec(flatSql))
    {
        m_lastError = query.lastError().text();

        qDebug() << "Create Flat Table Failed:"
                 << m_lastError;

        return false;
    }

    qDebug() << "Flat table created successfully.";

    if (!query.exec(MeterReadingSql::createTable()))
    {
        m_lastError = query.lastError().text();

        qDebug() << "Create MeterReading Table Failed:"
                 << m_lastError;

        return false;
    }

    qDebug() << "MeterReading table created successfully.";

    if (!query.exec(BillSql::createTable()))
    {
        m_lastError = query.lastError().text();

        qDebug() << "Create Bill Table Failed:"
                 << m_lastError;

        return false;
    }

    qDebug() << "Bill table created successfully.";

    return true;
}

bool AMSDatabase::isOpen() const
{
    return m_db.isOpen();
}

QSqlQuery AMSDatabase::executeQuery(const QString &sql)
{
    QSqlQuery query(m_db);

    if (!query.exec(sql))
    {
        m_lastError = query.lastError().text();

        qDebug() << "Query Failed :" << m_lastError;
    }

    return query;
}

bool AMSDatabase::executeNonQuery(const QString &sql)
{
    QSqlQuery query(m_db);

    if (!query.exec(sql))
    {
        m_lastError = query.lastError().text();

        qDebug() << "SQL Failed :" << m_lastError;

        return false;
    }

    return true;
}

bool AMSDatabase::executeNonQuery(QSqlQuery &query)
{
    if (!query.exec())
    {
        m_lastError = query.lastError().text();

        qDebug() << "SQL Failed :" << m_lastError;

        return false;
    }

    return true;
}


QString AMSDatabase::lastError() const
{
    return m_lastError;
}

QSqlDatabase AMSDatabase::database() const
{
    return m_db;
}


void AMSDatabase::close()
{
    if (m_db.isOpen())
    {
        m_db.close();

        qDebug() << "Database Closed.";
    }
}

