#include "Kernel.h"

namespace MCL
{

Kernel Kernel::create(const Program& program, const char* name) noexcept
{
    cl_int err;
    cl_kernel id = clCreateKernel(program.id(), name, &err);
    if (id == nullptr || err != CL_SUCCESS)
    {
        return Kernel(nullptr);
    }

    return Kernel(id);
}

Kernel::~Kernel() noexcept
{
    if (m_id == nullptr)
        return;

    clReleaseKernel(m_id);
    m_id = nullptr;
}

Kernel::Kernel(Kernel&& src) noexcept :
    m_id(src.m_id)
{
    src.m_id = nullptr;
}

Kernel::Kernel(cl_kernel id) :
    m_id(id)
{

}

cl_kernel Kernel::id() const noexcept
{
    return m_id;
}

cl_int Kernel::setArg(cl_uint index, const Buffer& data) const noexcept
{
    cl_mem mem = data.mem();
    return clSetKernelArg(m_id, index, sizeof(cl_mem), static_cast<void*>(&mem));
}

cl_int Kernel::invoke(const CommandQueue& cmd, size_t globalItems, size_t localItems) const noexcept
{
    return clEnqueueNDRangeKernel(cmd.id(), m_id, 1, nullptr, &globalItems, &localItems, 0, nullptr, nullptr);
}

}; // namespace MCL