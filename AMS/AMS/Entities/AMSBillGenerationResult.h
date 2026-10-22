#ifndef AMSBILLGENERATIONRESULT_H
#define AMSBILLGENERATIONRESULT_H

#include <QStringList>

class AMSBillGenerationResult
{
public:

    AMSBillGenerationResult();

    int occupiedFlats;

    int generatedBills;

    QStringList skippedFlats;
};

#endif // AMSBILLGENERATIONRESULT_H