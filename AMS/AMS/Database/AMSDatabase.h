#ifndef AMSDATABASE_H
#define AMSDATABASE_H

#include <QObject>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlQuery>


class AMSDatabase : public QObject
{
    Q_OBJECT

public:
    explicit AMSDatabase(QObject *parent = nullptr);
    ~AMSDatabase();

    bool open();
    void close();

    bool isOpen() const;

    bool execute(QSqlQuery &query);

    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    QSqlQuery executeQuery(const QString& sql);

    bool executeNonQuery(const QString& sql);
    bool executeNonQuery(QSqlQuery &query);
    QString lastError() const;
    QSqlDatabase database() const;

private:
    bool createTables();

private:
    QSqlDatabase m_db;
    QString m_lastError;
};

#endif // AMSDATABASE_H