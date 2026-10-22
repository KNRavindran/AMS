#ifndef AMSFLATVIEWMODEL_H
#define AMSFLATVIEWMODEL_H

#include <QObject>
#include "Models/AMSFlatListModel.h"

class AMSFlatManager;

class AMSFlatViewModel : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(
        AMSFlatListModel* flatModel
            READ flatModel
                CONSTANT)

    Q_PROPERTY(QString selectedFlatNo
                   READ selectedFlatNo
                       NOTIFY selectedFlatChanged)

    Q_PROPERTY(int selectedFlatType
                   READ selectedFlatType
                       NOTIFY selectedFlatChanged)

    Q_PROPERTY(int selectedStatus
                   READ selectedStatus
                       NOTIFY selectedFlatChanged)

    Q_PROPERTY(
        int selectedRow
            READ selectedRow
                NOTIFY selectedFlatChanged)

    Q_PROPERTY(
        int flatCount
            READ flatCount
                NOTIFY flatCountChanged)

    Q_INVOKABLE void selectFlat(int index);
    Q_INVOKABLE QStringList flatList() const;
    Q_INVOKABLE void clearSelection();

    explicit AMSFlatViewModel(
        AMSFlatManager *flatManager,
        QObject *parent = nullptr);

    Q_INVOKABLE bool addFlat(
        const QString& flatNo,
        int flatType,
        int status);

    Q_INVOKABLE bool updateFlat(
        const QString& flatNo,
        int flatType,
        int status);

    Q_INVOKABLE bool deleteFlat(
        const QString& flatNo);

    QString selectedFlatNo() const;

    int selectedFlatType() const;

    int selectedStatus() const;

    int selectedRow() const;

    AMSFlatListModel* flatModel() const;
    int flatCount() const;

signals:

    void operationCompleted(
        bool success,
        QString message);

    void selectedFlatChanged();

    void flatCountChanged();

private:
    int m_selectedRow;
    QString m_selectedFlatNo;

    int m_selectedFlatType;

    int m_selectedStatus;

    AMSFlatManager *m_flatManager;
};

#endif // AMSFLATVIEWMODEL_H