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
* @param[in] props context properties
* @param[in] pfn_notify callback
* @param[in] userdata userdata for pfn_notify
* @return the context
************************************************************/
Context Context::Create(const Device& dev, 
                        const cl_context_properties* props = nullptr, 
                        pfn_notify_t pfn_notify = nullptr,
                        void* userdata = nullptr) noexcept
{
    cl_int err;
    cl_device_id did = dev.id();
    cl_context ctx = clCreateContext(props, 1, &did, pfn_notify, userdata, &err);
    return Context(ctx);
}

}; // namespace MCL