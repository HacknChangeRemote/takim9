#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <harfcore/core/device/hdevicefactory.h>
#include "androidhelper.h"
#include <iostream>

#include "hdata.h"

using namespace harf::utils::device;

int main(int argc, char *argv[])
{    
#if defined(__ANDROID__)
    AndroidHelper::initialize();
#endif

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    HData* tData = new HData();
    engine.rootContext()->setContextProperty("mainData", tData);

    engine.load(QUrl("qrc:/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    HIDevice* tDevice = HDeviceFactory::getDevice();
    tData->setModel(QString::fromStdString(tDevice->getModel()));
    tData->setManufacturer(QString::fromStdString(tDevice->getManufacturer()));
    tData->setDataPath(QString::fromStdString(tDevice->getDataPath()));

    QString tCatStr = "Unknown";

    int tCategory = tDevice->getDeviceCategory();
    if(tCategory & HIDevice::DC_Windows)
        tCatStr = "Windows";
    else if(tCategory & HIDevice::DC_Android)
        tCatStr = "Android";
    else if(tCategory & HIDevice::DC_iOS)
        tCatStr = "iOS";
    else if(tCategory & HIDevice::DC_Linux)
        tCatStr = "Linux";
    else if(tCategory & HIDevice::DC_EmbeddedLinux)
        tCatStr = "Embedded Linux";

    if(tCategory & HIDevice::DC_NoDisplay)
        tCatStr += ", No-Display";
    else if(tCategory & HIDevice::DC_MonoDisplay)
        tCatStr += ", Video See-Thru";
    else if(tCategory & HIDevice::DC_StereoDisplay)
        tCatStr += ", Optical See-Thru";

    if(tCategory & HIDevice::DC_PC)
        tCatStr += ", PC";
    else if(tCategory & HIDevice::DC_Tablet)
        tCatStr += ", Tablet";
    else if(tCategory & HIDevice::DC_CellPhone)
        tCatStr += ", Cell Phone";
    else if(tCategory & HIDevice::DC_Glass)
        tCatStr += ", Glass";

    tData->setDeviceType(tCatStr);

    return app.exec();
}

