#ifndef METERREADINGSQL_H
#define METERREADINGSQL_H

#include <QString>

class MeterReadingSql
{
public:

    static QString createTable();

    static QString selectAll();

    static QString insert();

    static QString update();

    static QString remove();
};

#endif // METERREADINGSQL_H