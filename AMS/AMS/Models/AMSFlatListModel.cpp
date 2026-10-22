#include "AMSFlatListModel.h"

AMSFlatListModel::AMSFlatListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int AMSFlatListModel::rowCount(
    const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_flats.count();
}

QVariant AMSFlatListModel::data(
    const QModelIndex &index,
    int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.row() < 0 ||
        index.row() >= m_flats.count())
    {
        return QVariant();
    }

    AMSFlat *flat =
        m_flats.at(index.row());

    switch(role)
    {
    case FlatNoRole:
        return flat->flatNo();

    case FlatTypeRole:
        return AMSFlat::flatTypeToString(
            flat->flatType());

    case StatusRole:
        return flat->status() ==
                       AMSFlat::Occupied ?
                   "Occupied" :
                   "Vacant";
    }

    return QVariant();
}

QHash<int, QByteArray>
AMSFlatListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[FlatNoRole] = "flatNo";

    roles[FlatTypeRole] = "flatType";

    roles[StatusRole] = "status";

    return roles;
}

void AMSFlatListModel::setFlats(
    const QVector<AMSFlat*>& flats)
{
    beginResetModel();

    m_flats = flats;

    endResetModel();
}

AMSFlat *AMSFlatListModel::flatAt(
    int row) const
{
    if (row < 0 ||
        row >= m_flats.count())
    {
        return nullptr;
    }

    return m_flats.at(row);
}

void AMSFlatListModel::refresh()
{
    if (m_flats.isEmpty())
    {
        return;
    }

    emit dataChanged(
        index(0, 0),
        index(rowCount() - 1, 0));
}