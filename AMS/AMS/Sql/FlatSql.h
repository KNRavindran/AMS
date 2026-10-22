#ifndef FLATSQL_H
#define FLATSQL_H

#include <QString>

class FlatSql
{
public:

    static QString selectAll();

    static QString insert();

    static QString update();

    static QString remove();

    static QString exists();
};

#endif