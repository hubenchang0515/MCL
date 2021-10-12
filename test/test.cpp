#include "Tensor.h"

MCL::Device getGpu()
{
    auto plats = MCL::Platform::get();
    for (auto& plat : plats)
    {
        auto devs = MCL::Device::get(plat, CL_DEVICE_TYPE_GPU);
        for (auto& dev : devs)
        {
            return dev;
        }
    }
    
    return MCL::Device::invalid;
}


int main()
{
    auto gpu = getGpu();
    if(!Tensor::init(gpu))
    {
        fprintf(stderr, "init failed\n");
        return 1;
    }

    Tensor t1({2,2});
    Tensor t2({2,2});

    float (*pt1) [2][2] = reinterpret_cast<float (*)[2][2]>(t1.data());
    (*pt1)[0][0] = 1;
    (*pt1)[0][1] = 2;
    (*pt1)[1][0] = 3;
    (*pt1)[1][1] = 4;
    t1.printData();

    float (*pt2) [2][2] = reinterpret_cast<float (*)[2][2]>(t2.data());
    (*pt2)[0][0] = 4;
    (*pt2)[0][1] = 3;
    (*pt2)[1][0] = 2;
    (*pt2)[1][1] = 1;
    t1.printData();
    
    auto t3 = t1.add(t2);
    t3.printData();
}