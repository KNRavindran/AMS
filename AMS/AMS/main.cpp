#include <QResource>
#include <QDebug>
#include <QImage>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include<QPixmap>
#include <QDirIterator>
#include "Database/AMSDatabase.h"
#include "Managers/AMSFlatManager.h"
#include "Entities/AMSFlat.h"
#include "Managers/AMSMeterReadingManager.h"
#include "ViewModels/AMSFlatViewModel.h"
#include "Managers/AMSBillManager.h"
#include "ViewModels/AMSMeterReadingViewModel.h"
#include "ViewModels/AMSBillViewModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QDirIterator it(":", QDirIterator::Subdirectories);

    qDebug() << "====== Registered Resources ======";

    while (it.hasNext())
    {
        qDebug() << it.next();
    }
    qDebug() << "Resource Exists :"
             << QResource(":/Resources/Images/PaymentQR.png").isValid();

    QPixmap pix(":/Resources/Images/PaymentQR.png");

    qDebug() << "Pixmap Null =" << pix.isNull();
    qDebug() << "Pixmap Size =" << pix.size();

    QImage qrImage = pix.toImage();

    qDebug() << "Image Null =" << qrImage.isNull();

    AMSDatabase database;

    if (!database.open())
    {
        qDebug() << database.lastError();

        return -1;
    }
    //

    AMSFlatManager flatManager(&database);

    AMSMeterReadingManager meterReadingManager;

    AMSBillManager billManager;

    AMSBillViewModel billViewModel(
        &billManager,
        &flatManager,
        &meterReadingManager);



    AMSFlatViewModel flatViewModel(
        &flatManager);

    AMSMeterReadingViewModel meterReadingViewModel(
        &meterReadingManager);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.rootContext()->setContextProperty(
        "flatViewModel",
        &flatViewModel);

    engine.rootContext()->setContextProperty(
        "meterReadingViewModel",
        &meterReadingViewModel);

    engine.rootContext()->setContextProperty(
        "billViewModel",
        &billViewModel);

    engine.loadFromModule("AMS", "Main");

    return QCoreApplication::exec();
}
