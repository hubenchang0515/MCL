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
#include <memory>

namespace MCL
{

using pfn_notify_t = void (CL_CALLBACK*)(const char*, const void*, size_t, void*);
class Context
{
public:
    ~Context() = default;
    Context(const Context&) = default;
    Context(Context&& src) = default;

    Context& operator = (const Context&) = default;
    Context& operator = (Context&& src) = default;

    static const Context invalid;

    /************************************************************
    * @brief create context on a device
    * @param[in] dev the device
    * @param[in] props context properties
    * @param[in] pfn_notify callback
    * @param[in] userdata userdata for pfn_notify
    * @return the context
    ************************************************************/
    static Context create(const Device& dev, 
                            const cl_context_properties* props = nullptr, 
                            pfn_notify_t pfn_notify = nullptr,
                            void* userdata = nullptr) noexcept;
    
    cl_context id() const noexcept;
    std::string info(cl_context_info iname) const noexcept;


private:
    std::shared_ptr<cl_context> m_id;
    Context(cl_context id) noexcept;
};

}; // namespace MCL

#endif // MCL_CONTEXT_H