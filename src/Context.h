#ifndef MCL_CONTEXT_H
#define MCL_CONTEXT_H

#ifndef CL_TARGET_OPENCL_VERSION 
#define CL_TARGET_OPENCL_VERSION 220
#endif // CL_TARGET_OPENCL_VERSION

#include <vector>
#include <string>
#include <CL/cl.hpp>
#include <cstddef>
#include "Device.h"

namespace MCL
{

class Context
{
public:
    ~Context() noexcept;
    Context(const Context&) = delete;
    Context(Context&& src) noexcept;

    /************************************************************
    * @brief create context on a device
    * @param[in] dev the device
    * @return the context
    ************************************************************/
    static Context Create(const Device& dev) noexcept;

private:
    cl_context m_context;
    Context(cl_context ctx) noexcept;
};

}; // namespace MCL

#endif // MCL_CONTEXT_H