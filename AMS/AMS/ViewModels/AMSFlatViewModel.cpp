#include "AMSFlatViewModel.h"

#include "Managers/AMSFlatManager.h"
#include "Entities/AMSFlat.h"

AMSFlatViewModel::AMSFlatViewModel(
    AMSFlatManager *flatManager,
    QObject *parent)
    : QObject(parent),
    m_flatManager(flatManager),
    m_selectedFlatType(0),
    m_selectedStatus(0),
    m_selectedRow(-1)
{
}

AMSFlatListModel* AMSFlatViewModel::flatModel() const
{
    return m_flatManager->flatListModel();
}

bool AMSFlatViewModel::addFlat(
    const QString &flatNo,
    int flatType,
    int status)
{
    bool success =
        m_flatManager->addFlat(
            flatNo,
            static_cast<AMSFlat::FlatType>(flatType),
            static_cast<AMSFlat::Status>(status));

    emit operationCompleted(
        success,
        success ?
            "Flat added successfully."
                : "Failed to add flat.");
    if(success)
    {
        emit flatCountChanged();
    }
    return success;
}

bool AMSFlatViewModel::updateFlat(
    const QString &flatNo,
    int flatType,
    int status)
{
    bool success =
        m_flatManager->updateFlat(
            flatNo,
            static_cast<AMSFlat::FlatType>(flatType),
            static_cast<AMSFlat::Status>(status));

    emit operationCompleted(
        success,
        success ?
            "Flat updated successfully."
                : "Failed to update flat.");
    if(success)
    {
        emit flatCountChanged();
    }
    return success;
}

bool AMSFlatViewModel::deleteFlat(
    const QString &flatNo)
{
    bool success =
        m_flatManager->deleteFlat(flatNo);

    emit operationCompleted(
        success,
        success ?
            "Flat deleted successfully."
                : "Failed to delete flat.");

    if(success)
    {
        emit flatCountChanged();
    }

    return success;
}

QStringList AMSFlatViewModel::flatList() const
{
    QStringList list;

    QList<AMSFlat*> flats =
        m_flatManager->flats();

    for (AMSFlat *flat : flats)
    {
        QString row =
            flat->flatNo()
            + ","
            + AMSFlat::flatTypeToString(
                flat->flatType())
            + ","
            + (flat->status() ==
                       AMSFlat::Occupied ?
                   "Occupied"
                   : "Vacant");

        list.append(row);
    }

    return list;
}

QString AMSFlatViewModel::selectedFlatNo() const
{
    return m_selectedFlatNo;
}

int AMSFlatViewModel::selectedFlatType() const
{
    return m_selectedFlatType;
}

int AMSFlatViewModel::selectedStatus() const
{
    return m_selectedStatus;
}

void AMSFlatViewModel::selectFlat(int index)
{
    m_selectedRow = index;
    AMSFlat *flat =
        m_flatManager->flatListModel()->flatAt(index);

    if (!flat)
    {
        return;
    }

    m_selectedFlatNo = flat->flatNo();

    m_selectedFlatType =
        static_cast<int>(flat->flatType());

    m_selectedStatus =
        static_cast<int>(flat->status());

    emit selectedFlatChanged();
    m_flatManager->flatListModel()->refresh();
}

int AMSFlatViewModel::selectedRow() const
{
    return m_selectedRow;
}

void AMSFlatViewModel::clearSelection()
{
    qDebug() << "clearSelection() called";

    m_selectedRow = -1;

    m_selectedFlatNo.clear();

    m_selectedFlatType = 0;

    m_selectedStatus = 0;

    emit selectedFlatChanged();
    m_flatManager->flatListModel()->refresh();
}

int AMSFlatViewModel::flatCount() const
{
    return m_flatManager->flats().count();
}