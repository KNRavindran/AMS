#ifndef AMSBILLVIEWMODEL_H
#define AMSBILLVIEWMODEL_H

//#include <QObject>
#include <QSet>
#include "Models/AMSBillListModel.h"

class AMSBillManager;
class AMSFlatManager;
class AMSMeterReadingManager;

class AMSBillViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(
        AMSBillListModel* billModel
            READ billModel
                CONSTANT)

    Q_PROPERTY(
        QString selectedFlatNo
            READ selectedFlatNo
                NOTIFY selectedBillChanged)

    Q_PROPERTY(
        int selectedMonth
            READ selectedMonth
                NOTIFY selectedBillChanged)

    Q_PROPERTY(
        int selectedYear
            READ selectedYear
                NOTIFY selectedBillChanged)

    Q_PROPERTY(
        int selectedRow
            READ selectedRow
                NOTIFY selectedBillChanged)
    Q_PROPERTY(QString filterFlatNo
                   READ filterFlatNo
                       WRITE setFilterFlatNo
                           NOTIFY filterChanged)

    Q_PROPERTY(int filterMonth
                   READ filterMonth
                       WRITE setFilterMonth
                           NOTIFY filterChanged)

    Q_PROPERTY(int filterYear
                   READ filterYear
                       WRITE setFilterYear
                           NOTIFY filterChanged)
    Q_PROPERTY(
        AMSBillListModel* generatedBillModel
            READ generatedBillModel
                CONSTANT)
    Q_PROPERTY(
        int billCount
            READ billCount
                NOTIFY billCountChanged)
    Q_PROPERTY(
        int selectedCount
            READ selectedCount
                NOTIFY selectedRowsChanged)

    Q_PROPERTY(bool allSelected
                   READ isAllSelected
                       NOTIFY selectedRowsChanged)

    //Q_INVOKABLE void selectAllBills();

    //Q_INVOKABLE void clearAllSelections();

    Q_INVOKABLE bool isAllSelected() const;
public:

    explicit AMSBillViewModel(
        AMSBillManager *billManager,
        AMSFlatManager *flatManager,
        AMSMeterReadingManager *meterReadingManager,
        QObject *parent = nullptr);

    AMSBillListModel* billModel() const;

    Q_INVOKABLE void selectBill(int index);

    //Q_INVOKABLE void clearSelection();
    Q_INVOKABLE bool generateBills(
        int billMonth,
        int billYear,
        double ratePerUnit,
        double maintenance1BHK,
        double maintenance2BHK,
        double maintenance3BHK);

    //Q_INVOKABLE bool deleteSelectedBill();
    Q_INVOKABLE bool deleteSelectedBills();

    Q_INVOKABLE bool exportSelectedBillToPdf();

    Q_INVOKABLE void toggleSelection(int row);

    Q_INVOKABLE bool isSelected(int row) const;

    Q_INVOKABLE bool exportSelectedBills();

     Q_INVOKABLE void clearAllSelections();
    QString selectedFlatNo() const;
    Q_INVOKABLE void selectAllBills();
    int selectedMonth() const;

    int selectedYear() const;

    int selectedRow() const;

    QString filterFlatNo() const;
    int filterMonth() const;
    int filterYear() const;

    void setFilterFlatNo(const QString &value);
    void setFilterMonth(int value);
    void setFilterYear(int value);

    Q_INVOKABLE void applyFilter();
    Q_INVOKABLE void clearFilter();
    AMSBillListModel* generatedBillModel() const;
    int billCount() const;
    int selectedCount() const;
signals:

    void operationCompleted(
        bool success,
        QString message);

    void selectedBillChanged();

    void filterChanged();
    void billCountChanged();
    void selectedSelectionChanged();
    void selectedRowsChanged();
private:

    AMSBillManager *m_billManager;

    AMSFlatManager *m_flatManager;

    AMSMeterReadingManager *m_meterReadingManager;

    QString m_selectedFlatNo;

    int m_selectedMonth;

    int m_selectedYear;

    int m_selectedRow;
    //QSet<int> m_selectedRows;

    QString m_filterFlatNo;

    int m_filterMonth;

    int m_filterYear;

};

#endif