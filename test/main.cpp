#include "Platform.h"
#include "Device.h"

int main()
{
    auto plats = MCL::Platform::get();
    for (auto& plat : plats)
    {

        printf("Platform Name: %s\n", plat.name().c_str());
        printf("Platform Profile: %s\n", plat.profile().c_str());
        printf("Platform Version: %s\n", plat.version().c_str());
        printf("Platform Vendor: %s\n", plat.vendor().c_str());
        printf("Platform Extensions: %s\n", plat.extensions().c_str());
        auto devs = MCL::Device::get(plat, CL_DEVICE_TYPE_ALL);
        for (auto& dev : devs)
        {
            printf("Device Name: %s\n", dev.info(CL_DEVICE_NAME).c_str());
        }
        printf("\n");
    }
}