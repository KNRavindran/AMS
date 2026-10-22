#ifndef AMSBILL_H
#define AMSBILL_H

#include <QObject>
#include <QString>

class AMSBill : public QObject
{
    Q_OBJECT

public:

    explicit AMSBill(
        const QString& flatNo,
        int billMonth,
        int billYear,
        int previousReading,
        int currentReading,
        int unitsConsumed,
        double ratePerUnit,
        double electricityAmount,
        double maintenanceAmount,
        double totalAmount,
        QObject *parent = nullptr);

    QString flatNo() const;

    int billMonth() const;

    int billYear() const;

    int previousReading() const;

    int currentReading() const;

    int unitsConsumed() const;

    double ratePerUnit() const;

    double electricityAmount() const;

    double maintenanceAmount() const;

    double totalAmount() const;

    bool selected() const;

    void setSelected(bool value);
private:

    QString m_flatNo;

    int m_billMonth;

    int m_billYear;

    int m_previousReading;

    int m_currentReading;

    int m_unitsConsumed;

    double m_ratePerUnit;

    double m_electricityAmount;

    double m_maintenanceAmount;

    double m_totalAmount;

    bool m_selected = false;
};

#endif // AMSBILL_H