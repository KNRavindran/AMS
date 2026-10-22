#ifndef AMSFLATLISTMODEL_H
#define AMSFLATLISTMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "Entities/AMSFlat.h"

class AMSFlatListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    enum FlatRoles
    {
        FlatNoRole = Qt::UserRole + 1,
        FlatTypeRole,
        StatusRole
    };

    explicit AMSFlatListModel(
        QObject *parent = nullptr);

    int rowCount(
        const QModelIndex &parent =
        QModelIndex()) const override;

    QVariant data(
        const QModelIndex &index,
        int role) const override;

    QHash<int, QByteArray> roleNames()
        const override;

    void setFlats(
        const QVector<AMSFlat*>& flats);

    AMSFlat* flatAt(
        int row) const;
    void refresh();
private:

    QVector<AMSFlat*> m_flats;
};

#endif // AMSFLATLISTMODEL_H