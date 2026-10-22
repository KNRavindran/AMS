
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QStringList>
#include <QPdfWriter>
#include <QPainter>
#include <QStandardPaths>
#include <QDir>
#include <QFont>
#include <QLocale>
#include <QDate>

#include "AMSBillManager.h"
#include "Sql/BillSql.h"
#include "Managers/AMSFlatManager.h"
#include "Managers/AMSMeterReadingManager.h"

AMSBillManager::AMSBillManager(QObject *parent)
    : QObject(parent)
{
    m_billListModel =
        new AMSBillListModel(this);
    loadBills();
}

AMSBillManager::~AMSBillManager()
{
    qDeleteAll(m_bills);

    m_bills.clear();
}

bool AMSBillManager::billExists(
    const QString& flatNo,
    int billMonth,
    int billYear) const
{
    return findBill(
               flatNo,
               billMonth,
               billYear) != nullptr;
}

AMSBill* AMSBillManager::findBill(
    const QString& flatNo,
    int billMonth,
    int billYear) const
{
    QString trimmedFlatNo = flatNo.trimmed();

    for (AMSBill *bill : m_bills)
    {
        if (bill->flatNo() == trimmedFlatNo &&
            bill->billMonth() == billMonth &&
            bill->billYear() == billYear)
        {
            return bill;
        }
    }

    return nullptr;
}

bool AMSBillManager::loadBills()
{
    qDeleteAll(m_bills);

    m_bills.clear();

    QSqlQuery query(QSqlDatabase::database());

    if (!query.exec(BillSql::selectAll()))
    {
        qDebug() << "Load Bills Failed:"
                 << query.lastError().text();

        return false;
    }

    QSqlRecord record = query.record();

    int flatNoIndex =
        record.indexOf("FlatNo");

    int monthIndex =
        record.indexOf("BillMonth");

    int yearIndex =
        record.indexOf("BillYear");

    int previousReadingIndex =
        record.indexOf("PreviousReading");

    int currentReadingIndex =
        record.indexOf("CurrentReading");

    int unitsIndex =
        record.indexOf("Units");

    int rateIndex =
        record.indexOf("RatePerUnit");

    int electricityIndex =
        record.indexOf("ElectricityAmount");

    int maintenanceIndex =
        record.indexOf("MaintenanceAmount");

    int totalIndex =
        record.indexOf("TotalAmount");

    while (query.next())
    {
        AMSBill *bill =
            new AMSBill(
                query.value(flatNoIndex).toString(),
                query.value(monthIndex).toInt(),
                query.value(yearIndex).toInt(),
                query.value(previousReadingIndex).toInt(),
                query.value(currentReadingIndex).toInt(),
                query.value(unitsIndex).toInt(),
                query.value(rateIndex).toDouble(),
                query.value(electricityIndex).toDouble(),
                query.value(maintenanceIndex).toDouble(),
                query.value(totalIndex).toDouble(),
                this);

        m_bills.append(bill);
    }

    m_billListModel->setBills(
        QVector<AMSBill*>::fromList(m_bills));

    qDebug() << "Loaded Bills:"
             << m_bills.count();

    return true;
}

bool AMSBillManager::addBill(
    const QString& flatNo,
    int billMonth,
    int billYear,
    int previousReading,
    int currentReading,
    int units,
    double ratePerUnit,
    double electricityAmount,
    double maintenanceAmount,
    double totalAmount)
{
    QString trimmedFlatNo = flatNo.trimmed();

    if (trimmedFlatNo.isEmpty())
    {
        return false;
    }

    if (billMonth < 1 || billMonth > 12)
    {
        return false;
    }

    if (billYear < 2000)
    {
        return false;
    }

    if (previousReading < 0 ||
        currentReading < 0 ||
        units < 0)
    {
        return false;
    }

    if (ratePerUnit < 0.0 ||
        electricityAmount < 0.0 ||
        maintenanceAmount < 0.0 ||
        totalAmount < 0.0)
    {
        return false;
    }

    if (billExists(
            trimmedFlatNo,
            billMonth,
            billYear))
    {
        return false;
    }

    QSqlQuery query(QSqlDatabase::database());

    query.prepare(BillSql::insert());

    query.addBindValue(trimmedFlatNo);
    query.addBindValue(billMonth);
    query.addBindValue(billYear);
    query.addBindValue(previousReading);
    query.addBindValue(currentReading);
    query.addBindValue(units);
    query.addBindValue(ratePerUnit);
    query.addBindValue(electricityAmount);
    query.addBindValue(maintenanceAmount);
    query.addBindValue(totalAmount);

    if (!query.exec())
    {
        qDebug() << "Insert Bill Failed:"
                 << query.lastError().text();

        return false;
    }

    AMSBill *bill =
        new AMSBill(
            trimmedFlatNo,
            billMonth,
            billYear,
            previousReading,
            currentReading,
            units,
            ratePerUnit,
            electricityAmount,
            maintenanceAmount,
            totalAmount,
            this);

    m_bills.append(bill);

    m_billListModel->setBills(
        QVector<AMSBill*>::fromList(m_bills));

    qDebug() << "Bill Added Successfully.";

    return true;
}

bool AMSBillManager::updateBill(
    const QString& flatNo,
    int billMonth,
    int billYear,
    int previousReading,
    int currentReading,
    int units,
    double ratePerUnit,
    double electricityAmount,
    double maintenanceAmount,
    double totalAmount)
{
    QString trimmedFlatNo = flatNo.trimmed();

    if (trimmedFlatNo.isEmpty())
    {
        return false;
    }

    AMSBill *bill =
        findBill(
            trimmedFlatNo,
            billMonth,
            billYear);

    if (!bill)
    {
        return false;
    }

    QSqlQuery query(QSqlDatabase::database());

    query.prepare(BillSql::update());

    query.addBindValue(previousReading);
    query.addBindValue(currentReading);
    query.addBindValue(units);
    query.addBindValue(ratePerUnit);
    query.addBindValue(electricityAmount);
    query.addBindValue(maintenanceAmount);
    query.addBindValue(totalAmount);

    query.addBindValue(trimmedFlatNo);
    query.addBindValue(billMonth);
    query.addBindValue(billYear);

    if (!query.exec())
    {
        qDebug() << "Update Bill Failed:"
                 << query.lastError().text();

        return false;
    }

    m_bills.removeOne(bill);

    delete bill;

    AMSBill *updatedBill =
        new AMSBill(
            trimmedFlatNo,
            billMonth,
            billYear,
            previousReading,
            currentReading,
            units,
            ratePerUnit,
            electricityAmount,
            maintenanceAmount,
            totalAmount,
            this);

    m_bills.append(updatedBill);
    m_billListModel->setBills(
        QVector<AMSBill*>::fromList(m_bills));
    return true;
}

bool AMSBillManager::deleteBill(
    const QString& flatNo,
    int billMonth,
    int billYear)
{
    QString trimmedFlatNo = flatNo.trimmed();

    AMSBill *bill =
        findBill(
            trimmedFlatNo,
            billMonth,
            billYear);

    if (!bill)
    {
        return false;
    }

    QSqlQuery query(QSqlDatabase::database());

    query.prepare(BillSql::remove());

    query.addBindValue(trimmedFlatNo);
    query.addBindValue(billMonth);
    query.addBindValue(billYear);

    if (!query.exec())
    {
        qDebug() << "Delete Bill Failed:"
                 << query.lastError().text();

        return false;
    }

    m_bills.removeOne(bill);

    delete bill;

    m_billListModel->setBills(
        QVector<AMSBill*>::fromList(m_bills));

    return true;
}

void AMSBillManager::previousMonthYear(
    int currentMonth,
    int currentYear,
    int& previousMonth,
    int& previousYear) const
{
    if (currentMonth == 1)
    {
        previousMonth = 12;
        previousYear = currentYear - 1;
    }
    else
    {
        previousMonth = currentMonth - 1;
        previousYear = currentYear;
    }
}



bool AMSBillManager::generateBills(
    AMSFlatManager& flatManager,
    AMSMeterReadingManager& meterReadingManager,
    int billMonth,
    int billYear,
    double ratePerUnit,
    double maintenance1BHK,
    double maintenance2BHK,
    double maintenance3BHK,
    QStringList& skippedFlats,
    int& generatedCount)
{
    skippedFlats.clear();

    generatedCount = 0;
    //QVector<AMSBill*> generatedBills;

    QList<AMSFlat*> flats = flatManager.flats();

    for (AMSFlat *flat : flats)
    {
        // Generate bill only for occupied flats
        if (flat->status() != AMSFlat::Occupied)
        {
            continue;
        }

        AMSMeterReading *previousReading =
            meterReadingManager.previousReading(
                flat->flatNo(),
                billMonth,
                billYear);

        AMSMeterReading *currentReading =
            meterReadingManager.findMeterReading(
                flat->flatNo(),
                billMonth,
                billYear);

        if (!previousReading || !currentReading)
        {
            skippedFlats.append(flat->flatNo());

            qDebug() << "Skipping"
                     << flat->flatNo()
                     << "- Meter reading missing.";

            continue;
        }

        int units =
            currentReading->meterReading() -
            previousReading->meterReading();

        if (units < 0)
        {
            skippedFlats.append(flat->flatNo());

            qDebug() << "Skipping"
                     << flat->flatNo()
                     << "- Invalid readings.";

            continue;
        }

        double maintenanceAmount = 0.0;

        switch (flat->flatType())
        {
        case AMSFlat::Type1BHK:

            maintenanceAmount =
                maintenance1BHK;

            break;

        case AMSFlat::Type2BHK:

            maintenanceAmount =
                maintenance2BHK;

            break;

        case AMSFlat::Type3BHK:

            maintenanceAmount =
                maintenance3BHK;

            break;
        }

        double electricityAmount =
            units * ratePerUnit;

        double totalAmount =
            electricityAmount +
            maintenanceAmount;

        if (addBill(
                flat->flatNo(),
                billMonth,
                billYear,
                previousReading->meterReading(),
                currentReading->meterReading(),
                units,
                ratePerUnit,
                electricityAmount,
                maintenanceAmount,
                totalAmount))
        {
            generatedCount++;

            // AMSBill *bill =
            //     findBill(
            //         flat->flatNo(),
            //         billMonth,
            //         billYear);

            // if (bill)
            // {
            //     generatedBills.append(bill);
            // }

            qDebug()
                << "Bill generated for"
                << flat->flatNo();
        }
        else
        {
            skippedFlats.append(
                flat->flatNo());

            qDebug()
                << "Failed to generate bill for"
                << flat->flatNo();
        }
    }
    //m_generatedBillListModel->setBills(generatedBills);
    return true;
}

AMSBillListModel*
AMSBillManager::billListModel() const
{
    return m_billListModel;
}

bool AMSBillManager::filterBills(
    const QString& flatNo,
    int month,
    int year)
{
    QVector<AMSBill*> filteredBills;

    QString searchFlat =
        flatNo.trimmed();

    for (AMSBill *bill : m_bills)
    {
        bool match = true;

        if (!searchFlat.isEmpty())
        {
            if (bill->flatNo().compare(
                    searchFlat,
                    Qt::CaseInsensitive) != 0)
            {
                match = false;
            }
        }

        if (month > 0)
        {
            if (bill->billMonth() != month)
            {
                match = false;
            }
        }

        if (year > 0)
        {
            if (bill->billYear() != year)
            {
                match = false;
            }
        }

        if (match)
        {
            filteredBills.append(bill);
        }
    }

    m_billListModel->setBills(filteredBills);

    return true;
}
/*
AMSBillListModel*
AMSBillManager::generatedBillListModel() const
{
    return m_generatedBillListModel;
}*/
/*
bool AMSBillManager::exportBillToPdf(
    AMSBill *bill,
    const QString &fileName)
{

     QPdfWriter pdf(fullPath);

    pdf.setPageSize(QPageSize(QPageSize::A4));

    pdf.setResolution(300);

    QPainter painter(&pdf);

    if (!painter.isActive())
        return false;

    QFont titleFont("Arial",18,QFont::Bold);
    QFont headingFont("Arial",13,QFont::Bold);
    QFont normalFont("Arial",11);
    QFont totalFont("Arial",12,QFont::Bold);

    //
    const int left = 120;

    const int right = 2350;

    int y = 180;
    //
    if (!bill)
        return false;

    QString folder =
        QStandardPaths::writableLocation(
            QStandardPaths::DocumentsLocation);

    QString fullPath =
        QDir(folder).filePath(fileName);

    QPdfWriter pdf(fullPath);

    pdf.setPageSize(QPageSize(QPageSize::A4));

    pdf.setResolution(300);

    QPainter painter(&pdf);

    if (!painter.isActive())
        return false;

    QFont titleFont("Arial",18,QFont::Bold);
    painter.setFont(titleFont);

    painter.drawText(
        200,
        200,
        "Apartment Maintenance Bill");

    painter.setFont(QFont("Arial",11));


    painter.setFont(titleFont);

    painter.drawText(
        QRect(left,y,right-left,40),
        Qt::AlignCenter,
        "Apartment Management System");

    y += 70;

    painter.setFont(headingFont);

    painter.drawText(
        QRect(left,y,right-left,40),
        Qt::AlignCenter,
        "Monthly Maintenance Bill");

    y += 40;

    painter.drawLine(left,y,right,y);

    y += 70;
    int y = 450;

    painter.drawText(
        100,
        y,
        QString("Flat No : %1")
            .arg(bill->flatNo()));

    y += 120;

    painter.drawText(
        100,
        y,
        QString("Month : %1")
            .arg(bill->billMonth()));

    y += 120;

    painter.drawText(
        100,
        y,
        QString("Year : %1")
            .arg(bill->billYear()));

    y += 120;

    painter.drawText(
        100,
        y,
        QString("Previous Reading : %1")
            .arg(bill->previousReading()));

    y += 120;

    painter.drawText(
        100,
        y,
        QString("Current Reading : %1")
            .arg(bill->currentReading()));

    y += 120;

    painter.drawText(
        100,
        y,
        QString("Units Consumed : %1")
            .arg(bill->unitsConsumed()));

    y += 120;

    painter.drawText(
        100,
        y,
        QString("Rate Per Unit : %1")
            .arg(bill->ratePerUnit()));

    y += 120;

    painter.drawText(
        100,
        y,
        QString("Electricity : %1")
            .arg(bill->electricityAmount()));

    y += 120;

    painter.drawText(
        100,
        y,
        QString("Maintenance : %1")
            .arg(bill->maintenanceAmount()));

    y += 120;

    painter.setFont(QFont("Arial",13,QFont::Bold));

    painter.drawText(
        100,
        y,
        QString("Total Amount : %1")
            .arg(bill->totalAmount()));

    painter.end();

    return true;
}*/

bool AMSBillManager::exportBillToPdf(
    AMSBill *bill,
    const QString &fileName)
{
    if (!bill)
        return false;

    QString folder =
        QStandardPaths::writableLocation(
            QStandardPaths::DocumentsLocation);

    QString fullPath =
        QDir(folder).filePath(fileName);

    QPdfWriter pdf(fullPath);

    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter painter(&pdf);

    if (!painter.isActive())
        return false;

    //------------------------------------
    // Fonts
    //------------------------------------

    QFont titleFont("Arial",18,QFont::Bold);
    QFont headingFont("Arial",13,QFont::Bold);
    QFont normalFont("Arial",11);
    QFont totalFont("Arial",12,QFont::Bold);

    //------------------------------------
    // Page Margins
    //------------------------------------

    const int left = 120;
    const int right = 2350;

    int y = 180;

    //------------------------------------
    // Company Name
    //------------------------------------

    painter.setFont(titleFont);
    QFontMetrics fm(titleFont);

    int titleHeight = fm.height() + 20;

    painter.drawText(
        QRect(left,
              y,
              right-left,
              titleHeight),
        Qt::AlignHCenter | Qt::AlignVCenter,
        "Habulus Enclave III");

    y += titleHeight + 20;

    //------------------------------------
    // Bill Title
    //------------------------------------

    painter.setFont(headingFont);

    QFontMetrics fm2(headingFont);

    int headingHeight = fm2.height() + 40;

    painter.drawText(
        QRect(left,
              y,
              right-left,
              headingHeight),
        Qt::AlignHCenter | Qt::AlignVCenter,
        "Monthly Maintenance Bill");

    y += headingHeight + 40;

    painter.drawLine(left,y,right,y);

    y += 60;

    //------------------------------------
    // Flat Information
    //------------------------------------

    painter.setFont(normalFont);

    painter.drawText(left,y,
                     QString("Flat No : %1").arg(bill->flatNo()));

    painter.drawText(1200,y,
                     QString("Month : %1")
                         .arg(bill->billMonth()));

    y += 50;

    painter.drawText(left,y,
                     QString("Year : %1")
                         .arg(bill->billYear()));

    y += 70;

    //------------------------------------
    // Table
    //------------------------------------

    int x1 = left;
    int x2 = 850;
    int x3 = 1500;

    int rowHeight = 55;

    painter.setFont(headingFont);

    painter.drawRect(x1,y,700,rowHeight);
    painter.drawRect(x2,y,550,rowHeight);
    painter.drawRect(x3,y,500,rowHeight);

    painter.drawText(
        QRect(x1,y,700,rowHeight),
        Qt::AlignCenter,
        "Description");

    painter.drawText(
        QRect(x2,y,550,rowHeight),
        Qt::AlignCenter,
        "Value");

    painter.drawText(
        QRect(x3,y,500,rowHeight),
        Qt::AlignCenter,
        "Amount");

    painter.setFont(normalFont);

    auto drawRow =
        [&](const QString &desc,
            const QString &value,
            const QString &amount)
    {
        y += rowHeight;

        painter.drawRect(x1,y,700,rowHeight);
        painter.drawRect(x2,y,550,rowHeight);
        painter.drawRect(x3,y,500,rowHeight);

        painter.drawText(
            QRect(x1+10,y,680,rowHeight),
            Qt::AlignVCenter,
            desc);

        painter.drawText(
            QRect(x2+10,y,530,rowHeight),
            Qt::AlignVCenter,
            value);

        painter.drawText(
            QRect(x3+10,y,480,rowHeight),
            Qt::AlignVCenter,
            amount);
    };

    drawRow(
        "Previous Reading",
        QString::number(bill->previousReading()),
        "-");

    drawRow(
        "Current Reading",
        QString::number(bill->currentReading()),
        "-");

    drawRow(
        "Units Consumed",
        QString::number(bill->unitsConsumed()),
        "-");

    drawRow(
        "Rate Per Unit",
        QString::number(bill->ratePerUnit(),'f',2),
        "-");

    drawRow(
        "Electricity Charges",
        "",
        QString::number(
            bill->electricityAmount(),
            'f',
            2));

    drawRow(
        "Maintenance Charges",
        "",
        QString::number(
            bill->maintenanceAmount(),
            'f',
            2));

    //------------------------------------
    // Total
    //------------------------------------

    y += rowHeight + 20;

    painter.setFont(totalFont);

    painter.drawText(
        QRect(left,y,right-left,50),
        Qt::AlignRight,
        QString("TOTAL : %1")
            .arg(
                QString::number(
                    bill->totalAmount(),
                    'f',
                    2)));

    QPixmap qr(":/Resources/Images/PaymentQR.png");

    if (!qr.isNull())
    {
        QSize targetSize(850, 850);

        painter.drawPixmap(
            QRect(1450, 2200,
                  targetSize.width(),
                  targetSize.height()),
            qr);

        painter.drawText(
            QRect(1450,
                  3070,
                  850,
                  60),
            Qt::AlignCenter,
            "Scan to Pay");
    }
    //------------------------------------
    // Footer
    //------------------------------------

    y += 120;

    painter.setFont(QFont("Arial",10));

    painter.drawLine(left,y,right,y);

    y += 40;

    painter.drawText(
        QRect(left,y,right-left,40),
        Qt::AlignCenter,
        "This is a computer generated bill. No signature required.");

    painter.end();

    return true;
}