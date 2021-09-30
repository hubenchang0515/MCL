#include "MCL/Platform.h"
#include "MCL/Device.h"
#include "MCL/Context.h"
#include "MCL/CommandQueue.h"
#include "MCL/Program.h"
#include "MCL/Kernel.h"
#include "MCL/Buffer.h"

#define VECTOR_SIZE    1024

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
    if (gpu == MCL::Device::invalid)
    {
        fprintf(stderr, "failed to find GPU\n");
        return EXIT_FAILURE;
    }
    printf("Get GPU: %s\n", gpu.info(CL_DEVICE_NAME).c_str());
    
    auto ctx = MCL::Context::create(gpu);
    if (ctx == MCL::Context::invalid)
    {
        fprintf(stderr, "failed to create context\n");
        return EXIT_FAILURE;
    }
    
    auto cmd = MCL::CommandQueue::create(gpu, ctx);
    if (cmd == MCL::CommandQueue::invalid)
    {
        fprintf(stderr, "failed to create command queue\n");
        return EXIT_FAILURE;
    }

    auto program = MCL::Program::load(ctx, "kernel.cl");
    if (!program.build(gpu))
    {
        fprintf(stderr, "failed to build program:\n%s\n", program.info(gpu).c_str());
        return EXIT_FAILURE;
    }
    program.print();

    auto vecAdd = MCL::Kernel::create(program, "vecAdd");
    
    /* 创建数据 */
    cl_float* x = static_cast<float*>(malloc(VECTOR_SIZE * sizeof(cl_float)));
    cl_float* y = static_cast<float*>(malloc(VECTOR_SIZE * sizeof(cl_float)));
    if (x == nullptr || y == nullptr)
    {
        fprintf(stderr, "RAM bad alloc\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < VECTOR_SIZE; i++)
    {
        x[i] = 1.0f * i;
        y[i] = 2.0f * i;
    }

    auto X = MCL::Buffer::create(ctx, VECTOR_SIZE * sizeof(cl_float));
    auto Y = MCL::Buffer::create(ctx, VECTOR_SIZE * sizeof(cl_float));
    X.write(cmd, x,  VECTOR_SIZE * sizeof(cl_float));
    Y.write(cmd, y,  VECTOR_SIZE * sizeof(cl_float));

    vecAdd.setArg(0, X);
    vecAdd.setArg(1, Y);
    vecAdd.invoke(cmd, VECTOR_SIZE, 64);

    X.read(cmd, x, VECTOR_SIZE * sizeof(cl_float));
    for (int i = 0; i < VECTOR_SIZE; i++)
    {
        printf("%f  ", x[i]);
    }
    printf("\n");

    free(x);
    free(y);
    return EXIT_SUCCESS;
}