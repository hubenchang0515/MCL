#include "Context.h"

namespace MCL
{

Context::~Context() noexcept
{
    if(m_context == nullptr)
        return;

    clReleaseContext(m_context);
    m_context = nullptr;
}

Context::Context(cl_context ctx) noexcept :
    m_context(ctx)
{

}

Context::Context(Context&& src) noexcept :
    m_context(src.m_context)
{
    src.m_context = nullptr;
}

/************************************************************
* @brief create context on a device
* @param[in] dev the device
* @return the context
************************************************************/
Context Context::Create(const Device& dev) noexcept
{
    cl_int err;
    cl_device_id did = dev.id();
    cl_context ctx = clCreateContext(nullptr, 1, &did, nullptr, nullptr, &err);
    return Context(ctx);
}

}; // namespace MCL