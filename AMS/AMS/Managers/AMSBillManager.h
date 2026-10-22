#ifndef AMSBILLMANAGER_H
#define AMSBILLMANAGER_H

#include <QObject>
#include <QList>

#include <QStringList>

#include "Managers/AMSFlatManager.h"
#include "Managers/AMSMeterReadingManager.h"

#include "Entities/AMSBill.h"
#include "Entities/AMSBillGenerationResult.h"
#include "Models/AMSBillListModel.h"

class AMSBillManager : public QObject
{
    Q_OBJECT

public:

    explicit AMSBillManager(
        QObject *parent = nullptr);

    ~AMSBillManager();

    bool loadBills();
    bool filterBills(
        const QString& flatNo,
        int month,
        int year);

    bool addBill(
        const QString& flatNo,
        int billMonth,
        int billYear,
        int previousReading,
        int currentReading,
        int units,
        double ratePerUnit,
        double electricityAmount,
        double maintenanceAmount,
        double totalAmount);

    bool updateBill(
        const QString& flatNo,
        int billMonth,
        int billYear,
        int previousReading,
        int currentReading,
        int units,
        double ratePerUnit,
        double electricityAmount,
        double maintenanceAmount,
        double totalAmount);

    bool deleteBill(
        const QString& flatNo,
        int billMonth,
        int billYear);
    bool exportBillToPdf(
        const QString& flatNo,
        int billMonth,
        int billYear);
    bool billExists(
        const QString& flatNo,
        int billMonth,
        int billYear) const;

    AMSBill* findBill(
        const QString& flatNo,
        int billMonth,
        int billYear) const;

bool generateBills(
    AMSFlatManager& flatManager,
    AMSMeterReadingManager& meterReadingManager,
    int billMonth,
    int billYear,
    double ratePerUnit,
    double maintenance1BHK,
    double maintenance2BHK,
    double maintenance3BHK,
    QStringList& skippedFlats,
    int& generatedCount);

    bool exportBillToPdf(
        AMSBill *bill,
        const QString &fileName);
AMSBillListModel* billListModel() const;
//AMSBillListModel* generatedBillListModel() const;
private:
    void previousMonthYear(
        int currentMonth,
        int currentYear,
        int& previousMonth,
        int& previousYear) const;

    QList<AMSBill*> m_bills;
    AMSBillListModel *m_billListModel;
    //AMSBillListModel *m_generatedBillListModel;
};

#endif // AMSBILLMANAGER_H