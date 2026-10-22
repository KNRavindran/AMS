#ifndef AMSFLATMANAGER_H
#define AMSFLATMANAGER_H

#include <QObject>
#include <QVector>
#include "Entities/AMSFlat.h"
#include "Models/AMSFlatListModel.h"

class AMSDatabase;
class AMSFlat;

class AMSFlatManager : public QObject
{
    Q_OBJECT

public:

    explicit AMSFlatManager(AMSDatabase *database,
                            QObject *parent = nullptr);

    ~AMSFlatManager();
    AMSFlatListModel* flatListModel();

    Q_INVOKABLE bool addFlat(
        const QString& flatNo,
        AMSFlat::FlatType flatType,
        AMSFlat::Status status);

    Q_INVOKABLE bool updateFlat(
        const QString& flatNo,
        AMSFlat::FlatType flatType,
        AMSFlat::Status status);

    Q_INVOKABLE bool deleteFlat(
        const QString& flatNo);

    Q_INVOKABLE bool flatExists(
        const QString& flatNo)const;

    AMSFlat* findFlat(
        const QString& flatNo) const;

    bool loadFlats();
    const QList<AMSFlat*>& flats() const;


private:
    bool isValidFlatNo(
        const QString &flatNo,
        QString &trimmedFlatNo) const;

    AMSFlatListModel m_flatListModel;
    AMSDatabase *m_database;

    QVector<AMSFlat*> m_flats;
};

#endif // AMSFLATMANAGER_H