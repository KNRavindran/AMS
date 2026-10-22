#ifndef AMSBILLGENERATIONVIEWMODEL_H
#define AMSBILLGENERATIONVIEWMODEL_H

#include <QObject>
#include <QString>

class AMSBillManager;
class AMSFlatManager;
class AMSMeterReadingManager;

class AMSBillGenerationViewModel : public QObject
{
    Q_OBJECT

public:

    explicit AMSBillGenerationViewModel(
        AMSBillManager* billManager,
        AMSFlatManager* flatManager,
        AMSMeterReadingManager* meterReadingManager,
        QObject *parent =nullptr);

public slots:

    void generateBills(
        int month,
        int year,
        double ratePerUnit,
        double maintenance1BHK,
        double maintenance2BHK,
        double maintenance3BHK);

signals:

    void billGenerationCompleted(
        int generatedBills,
        int skippedBills,
        QString skippedFlatNumbers);

private:

    AMSBillManager* m_billManager;

    AMSFlatManager* m_flatManager;

    AMSMeterReadingManager* m_meterReadingManager;
};

#endif