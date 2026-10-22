#ifndef BILLSQL_H
#define BILLSQL_H

#include <QString>

class BillSql
{
public:

    static QString createTable();

    static QString selectAll();

    static QString insert();

    static QString update();

    static QString remove();
};

#endif // BILLSQL_H