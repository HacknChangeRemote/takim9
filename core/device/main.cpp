#include <harfcore/core/device/hdevicefactory.h>
#include "androidhelper.h"
#include <iostream>

using namespace harf::utils::device;

int main(int argc, char *argv[])
{
    (void) argc; //unused
    (void) argv;

#if defined(__ANDROID__)
    AndroidHelper::initialize();
#endif

    HIDevice* tDevice = HDeviceFactory::getDevice();

    std::cout << "H-ARF SDK Device Example" << std::endl;

    std::cout << "Manufacturer: " << tDevice->getManufacturer() << std::endl;
    std::cout << "Model: " << tDevice->getModel() << std::endl;

    std::cout << "Data Path: " << tDevice->getDataPath() << std::endl;

    std::string tCatStr = "Unknown";

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


    std::cout << "Category: " << tCatStr << std::endl;

#if !defined(__ANDROID__)
    return std::getchar();
#endif
}
