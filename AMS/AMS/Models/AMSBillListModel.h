#ifndef AMSBILLLISTMODEL_H
#define AMSBILLLISTMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "Entities/AMSBill.h"

class AMSBillListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    enum BillRoles
    {
        FlatNoRole = Qt::UserRole + 1,
        BillMonthRole,
        BillYearRole,
        PreviousReadingRole,
        CurrentReadingRole,
        UnitsConsumedRole,
        RatePerUnitRole,
        ElectricityAmountRole,
        MaintenanceAmountRole,
        TotalAmountRole,
        SelectedRole
    };

    explicit AMSBillListModel(
        QObject *parent = nullptr);

    int rowCount(
        const QModelIndex &parent =
        QModelIndex()) const override;

    QVariant data(
        const QModelIndex &index,
        int role) const override;

    QHash<int,QByteArray> roleNames()
        const override;

    void setBills(
        const QVector<AMSBill*>& bills);

    AMSBill* billAt(
        int row) const;

    void refresh();
    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void updateSelection(int row);
private:

    QVector<AMSBill*> m_bills;
};

#endif // AMSBILLLISTMODEL_H