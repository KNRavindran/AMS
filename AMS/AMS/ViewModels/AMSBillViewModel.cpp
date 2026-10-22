#include <QDate>
#include "AMSBillViewModel.h"

#include "Managers/AMSBillManager.h"
#include "Managers/AMSFlatManager.h"
#include "Managers/AMSMeterReadingManager.h"


AMSBillViewModel::AMSBillViewModel(
    AMSBillManager *billManager,
    AMSFlatManager *flatManager,
    AMSMeterReadingManager *meterReadingManager,
    QObject *parent)
    : QObject(parent),
    m_billManager(billManager),
    m_flatManager(flatManager),
    m_meterReadingManager(meterReadingManager),
    m_selectedMonth(1),
    m_selectedYear(QDate::currentDate().year()),
    m_selectedRow(-1)
{
    m_filterMonth = 0;
    m_filterYear = 0;
}

AMSBillListModel*
AMSBillViewModel::billModel() const
{
    return m_billManager->billListModel();
}

QString AMSBillViewModel::selectedFlatNo() const
{
    return m_selectedFlatNo;
}

int AMSBillViewModel::selectedMonth() const
{
    return m_selectedMonth;
}

int AMSBillViewModel::selectedYear() const
{
    return m_selectedYear;
}

int AMSBillViewModel::selectedRow() const
{
    return m_selectedRow;
}

void AMSBillViewModel::selectBill(int index)
{
    m_selectedRow = index;

    AMSBill *bill =
        m_billManager->billListModel()->billAt(index);

    if (!bill)
        return;

    m_selectedFlatNo = bill->flatNo();
    m_selectedMonth = bill->billMonth();
    m_selectedYear = bill->billYear();

    emit selectedBillChanged();

    m_billManager->billListModel()->refresh();
}

void AMSBillViewModel::clearAllSelections()
{
    int count =
        m_billManager
            ->billListModel()
            ->rowCount();

    for (int i = 0; i < count; ++i)
    {
        AMSBill *bill =
            m_billManager
                ->billListModel()
                ->billAt(i);

        if (!bill)
            continue;

        bill->setSelected(false);

        m_billManager
            ->billListModel()
            ->updateSelection(i);
    }

    emit selectedRowsChanged();
}

/*
void AMSBillViewModel::toggleSelection(int row)
{
    if (m_selectedRows.contains(row))
        m_selectedRows.remove(row);
    else
        m_selectedRows.insert(row);

    emit selectedSelectionChanged();
    m_billManager->billListModel()->refresh();


}
*/
/*
void AMSBillViewModel::toggleSelection(int row)
{
    AMSBill *bill =
        m_billManager->billListModel()->billAt(row);

    if (!bill)
        return;

    //---------------------------------------
    // Toggle model selection
    //---------------------------------------

    bool checked = !bill->selected();

    bill->setSelected(checked);

    //---------------------------------------
    // Keep QSet in sync (temporary)
    //---------------------------------------

    if (checked)
        m_selectedRows.insert(row);
    else
        m_selectedRows.remove(row);

    emit selectedRowsChanged();
    emit selectedSelectionChanged();

    m_billManager->billListModel()->refresh();
    qDebug() << "toggleSelection called row =" << row;
}
*/

void AMSBillViewModel::toggleSelection(int row)
{
    AMSBill *bill =
        m_billManager->billListModel()->billAt(row);

    if (!bill)
        return;

    qDebug()
        << "toggleSelection row="
        << row;
    bill->setSelected(!bill->selected());

    m_billManager
        ->billListModel()
        ->updateSelection(row);

    emit selectedRowsChanged();
}
/*
bool AMSBillViewModel::isSelected(int row) const
{
    return m_selectedRows.contains(row);
}*/

bool AMSBillViewModel::isSelected(int row) const
{
    AMSBill *bill =
        m_billManager->billListModel()->billAt(row);

    if (!bill)
        return false;

    return bill->selected();
}

bool AMSBillViewModel::generateBills(
    int billMonth,
    int billYear,
    double ratePerUnit,
    double maintenance1BHK,
    double maintenance2BHK,
    double maintenance3BHK)
{
    QStringList skippedFlats;

    int generatedCount = 0;

    bool success =
        m_billManager->generateBills(
            *m_flatManager,
            *m_meterReadingManager,
            billMonth,
            billYear,
            ratePerUnit,
            maintenance1BHK,
            maintenance2BHK,
            maintenance3BHK,
            skippedFlats,
            generatedCount);

    m_billManager->loadBills();
    clearAllSelections();;

    if (success)
    {
        emit operationCompleted(
            true,
            QString("Generated %1 bills.\nSkipped Flats: %2")
                .arg(generatedCount)
                .arg(skippedFlats.join(", ")));
        emit billCountChanged();
    }
    else
    {
        emit operationCompleted(
            false,
            "Bill generation failed.");
    }

    return success;
}

/*
bool AMSBillViewModel::deleteSelectedBill()
{
    if (m_selectedRow < 0)
    {
        emit operationCompleted(
            false,
            "No bill selected.");

        return false;
    }

    bool success =
        m_billManager->deleteBill(
            m_selectedFlatNo,
            m_selectedMonth,
            m_selectedYear);

    if (success)
    {
        m_billManager->loadBills();

        clearSelection();
        m_selectedRows.clear();

        emit operationCompleted(
            true,
            "Bill deleted successfully.");
        emit billCountChanged();
    }
    else
    {
        emit operationCompleted(
            false,
            "Unable to delete bill.");
    }

    return success;
}
*/

bool AMSBillViewModel::deleteSelectedBills()
{
    if (selectedCount() == 0)
    {
        emit operationCompleted(false,
                                "Please select at least one bill.");

        return false;
    }




    int deleted = 0;

    for (int row =
         m_billManager->billListModel()->rowCount()-1;
         row >= 0;
         row--)
    {
        AMSBill *bill =
            m_billManager->billListModel()->billAt(row);

        if (!bill)
            continue;

        if (!bill->selected())
            continue;

        if (m_billManager->deleteBill(
                bill->flatNo(),
                bill->billMonth(),
                bill->billYear()))
        {
            deleted++;
        }
    }

    //m_selectedRows.clear();

    m_billManager->loadBills();


    clearAllSelections();

    emit billCountChanged();

    emit operationCompleted(
        true,
        QString("%1 bill(s) deleted successfully.")
            .arg(deleted));

    return true;
}

QString AMSBillViewModel::filterFlatNo() const
{
    return m_filterFlatNo;
}

int AMSBillViewModel::filterMonth() const
{
    return m_filterMonth;
}

int AMSBillViewModel::filterYear() const
{
    return m_filterYear;
}

void AMSBillViewModel::setFilterFlatNo(const QString &value)
{
    if (m_filterFlatNo == value)
        return;

    m_filterFlatNo = value;

    emit filterChanged();
}

void AMSBillViewModel::setFilterMonth(int value)
{
    if (m_filterMonth == value)
        return;

    m_filterMonth = value;

    emit filterChanged();
}

void AMSBillViewModel::setFilterYear(int value)
{
    if (m_filterYear == value)
        return;

    m_filterYear = value;

    emit filterChanged();
}

void AMSBillViewModel::applyFilter()
{
    m_billManager->filterBills(
        m_filterFlatNo,
        m_filterMonth,
        m_filterYear);
}

void AMSBillViewModel::clearFilter()
{
    m_filterFlatNo.clear();

    m_filterMonth = 0;

    m_filterYear = 0;

    emit filterChanged();

    m_billManager->loadBills();
}

AMSBillListModel*
AMSBillViewModel::generatedBillModel() const
{
    return m_billManager->billListModel();
}

int AMSBillViewModel::billCount() const
{
    return
        m_billManager
            ->billListModel()
            ->rowCount();
}

bool AMSBillViewModel::exportSelectedBillToPdf()
{
    if (selectedCount() == 0)
    {
        emit operationCompleted(
            false,
            "Please select at least one bill.");

        return false;
    }

    AMSBill *bill =
        m_billManager->findBill(
            m_selectedFlatNo,
            m_selectedMonth,
            m_selectedYear);

    if (!bill)
    {
        emit operationCompleted(
            false,
            "Selected bill not found.");

        return false;
    }

    QString fileName =
        QString("Bill_%1_%2_%3.pdf")
            .arg(bill->flatNo())
            .arg(bill->billMonth())
            .arg(bill->billYear());

    bool success =
        m_billManager->exportBillToPdf(
            bill,
            fileName);

    emit operationCompleted(
        success,
        success ?
            QString("PDF exported successfully.\n%1")
                .arg(fileName)
                :
            "Unable to export PDF.");

    return success;
}

bool AMSBillViewModel::exportSelectedBills()
{
    if (selectedCount() == 0)
    {
        emit operationCompleted(
            false,
            "Please select at least one bill.");

        return false;
    }

    int exportedCount = 0;

    auto *model =
        m_billManager->billListModel();

    for (int row = 0;
         row < model->rowCount();
         row++)
    {
        AMSBill *bill =
            model->billAt(row);

        if (!bill)
            continue;

        if (!bill->selected())
            continue;

        QString fileName =
            QString("Bill_%1_%2_%3.pdf")
                .arg(bill->flatNo())
                .arg(bill->billMonth())
                .arg(bill->billYear());

        if (m_billManager->exportBillToPdf(
                bill,
                fileName))
        {
            exportedCount++;
        }
    }

    emit operationCompleted(
        true,
        QString("%1 PDF file(s) exported successfully.")
            .arg(exportedCount));

    return true;
}

/*
int AMSBillViewModel::selectedCount() const
{
    int count = 0;

    for(int i = 0;
         i < m_billManager->billListModel()->rowCount();
         i++)
    {
        AMSBill *bill =
            m_billManager->billListModel()->billAt(i);

        if(bill && bill->selected())
            count++;
    }

    return count;

    qDebug() << "selectAllBills called";
}*/

int AMSBillViewModel::selectedCount() const
{
    int count = 0;

    auto *model =
        m_billManager->billListModel();

    for (int i = 0; i < model->rowCount(); i++)
    {
        AMSBill *bill = model->billAt(i);

        if (bill && bill->selected())
            count++;
    }

    return count;
}

void AMSBillViewModel::selectAllBills()
{
    int count =
        m_billManager
            ->billListModel()
            ->rowCount();

    for (int i = 0; i < count; ++i)
    {
        AMSBill *bill =
            m_billManager
                ->billListModel()
                ->billAt(i);

        if (!bill)
            continue;

        bill->setSelected(true);

        m_billManager
            ->billListModel()
            ->updateSelection(i);
    }

    emit selectedRowsChanged();
    qDebug() << "clearAllSelections() called";
}

bool AMSBillViewModel::isAllSelected() const
{
    int count =
        m_billManager
            ->billListModel()
            ->rowCount();

    if (count == 0)
        return false;

    for (int i = 0; i < count; i++)
    {
        AMSBill *bill =
            m_billManager
                ->billListModel()
                ->billAt(i);

        if (!bill)
            continue;

        if (!bill->selected())
            return false;
    }

    return true;
}