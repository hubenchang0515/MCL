#include "Context.h"

namespace MCL
{

Context::Context(cl_context id) noexcept :
    m_id(new cl_context, [](cl_context* id){clReleaseContext(*id); delete id;})
{
    *m_id = id;
}

/************************************************************
* @brief create context on a device
* @param[in] dev the device
* @param[in] props context properties
* @param[in] pfn_notify callback
* @param[in] userdata userdata for pfn_notify
* @return the context
************************************************************/
Context Context::create(const Device& dev, 
                        const cl_context_properties* props, 
                        pfn_notify_t pfn_notify,
                        void* userdata) noexcept
{
    cl_int err;
    cl_device_id did = dev.id();
    cl_context ctx = clCreateContext(props, 1, &did, pfn_notify, userdata, &err);
    return Context(ctx);
}

cl_context Context::id() const noexcept
{
    return *m_id;
}

std::string Context::info(cl_context_info iname) const noexcept
{
    size_t n = 0;
    cl_int err = clGetContextInfo(*m_id, iname, 0, nullptr, &n); // n will include the '\0'
    if (err != CL_SUCCESS || n == 0)
    {
        return "";
    }

    char* name = new char[n];
    clGetContextInfo(*m_id, iname, n, name, nullptr);
    std::string ret = name;
    delete[] name;

    return ret;
}

}; // namespace MCL