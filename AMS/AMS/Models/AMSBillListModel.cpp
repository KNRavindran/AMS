#include "AMSBillListModel.h"

AMSBillListModel::AMSBillListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int AMSBillListModel::rowCount(
    const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_bills.count();
}

QVariant AMSBillListModel::data(
    const QModelIndex &index,
    int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 ||
        index.row() >= m_bills.count())
        return QVariant();

    AMSBill *bill = m_bills.at(index.row());

    switch(role)
    {
    case FlatNoRole:
        return bill->flatNo();

    case BillMonthRole:
        return bill->billMonth();

    case BillYearRole:
        return bill->billYear();

    case PreviousReadingRole:
        return bill->previousReading();

    case CurrentReadingRole:
        return bill->currentReading();

    case UnitsConsumedRole:
        return bill->unitsConsumed();

    case RatePerUnitRole:
        return bill->ratePerUnit();

    case ElectricityAmountRole:
        return bill->electricityAmount();

    case MaintenanceAmountRole:
        return bill->maintenanceAmount();

    case TotalAmountRole:
        return bill->totalAmount();
    case SelectedRole:
        return bill->selected();
    }

    return QVariant();
}

QHash<int,QByteArray>
AMSBillListModel::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[FlatNoRole] = "flatNo";
    roles[BillMonthRole] = "billMonth";
    roles[BillYearRole] = "billYear";
    roles[PreviousReadingRole] = "previousReading";
    roles[CurrentReadingRole] = "currentReading";
    roles[UnitsConsumedRole] = "unitsConsumed";
    roles[RatePerUnitRole] = "ratePerUnit";
    roles[ElectricityAmountRole] = "electricityAmount";
    roles[MaintenanceAmountRole] = "maintenanceAmount";
    roles[TotalAmountRole] = "totalAmount";
    roles[SelectedRole] = "selected";

    return roles;
}

void AMSBillListModel::setBills(
    const QVector<AMSBill *> &bills)
{
    beginResetModel();

    m_bills = bills;

    endResetModel();
}

AMSBill*
AMSBillListModel::billAt(
    int row) const
{
    if (row < 0 ||
        row >= m_bills.count())
    {
        return nullptr;
    }

    return m_bills.at(row);
}

void AMSBillListModel::refresh()
{
    if (m_bills.isEmpty())
        return;

    emit dataChanged(
        index(0),
        index(m_bills.count()-1));
}

bool AMSBillListModel::setData(
    const QModelIndex &index,
    const QVariant &value,
    int role)
{
    if (!index.isValid())
        return false;

    AMSBill *bill = billAt(index.row());

    if (!bill)
        return false;

    switch(role)
    {
    case SelectedRole:

        bill->setSelected(value.toBool());

        emit dataChanged(
            index,
            index,
            {SelectedRole});

        return true;

    default:
        return false;
    }
}

Qt::ItemFlags AMSBillListModel::flags(
    const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled
           | Qt::ItemIsSelectable
           | Qt::ItemIsEditable;
}

void AMSBillListModel::updateSelection(int row)
{
    QModelIndex idx = index(row);

    emit dataChanged(idx,
                     idx,
                     {SelectedRole});
}