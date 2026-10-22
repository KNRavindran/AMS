#include "AMSBillGenerationViewModel.h"

#include "Managers/AMSBillManager.h"
#include "Managers/AMSFlatManager.h"
#include "Managers/AMSMeterReadingManager.h"

AMSBillGenerationViewModel::
    AMSBillGenerationViewModel(
        AMSBillManager* billManager,
        AMSFlatManager* flatManager,
        AMSMeterReadingManager* meterReadingManager,
        QObject *parent)
    : QObject(parent),
    m_billManager(billManager),
    m_flatManager(flatManager),
    m_meterReadingManager(meterReadingManager)
{
}

void AMSBillGenerationViewModel::generateBills(
    int month,
    int year,
    double ratePerUnit,
    double maintenance1BHK,
    double maintenance2BHK,
    double maintenance3BHK)
{
    QStringList skippedFlats;

    int generatedBills = 0;

    m_billManager->generateBills(
        *m_flatManager,
        *m_meterReadingManager,
        month,
        year,
        ratePerUnit,
        maintenance1BHK,
        maintenance2BHK,
        maintenance3BHK,
        skippedFlats,
        generatedBills);

    emit billGenerationCompleted(
        generatedBills,
        skippedFlats.count(),
        skippedFlats.join(", "));
}