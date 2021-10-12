#ifndef MCL_COMMANDQUEUE_H
#define MCL_COMMANDQUEUE_H

#ifndef CL_TARGET_OPENCL_VERSION 
#define CL_TARGET_OPENCL_VERSION 220
#endif // CL_TARGET_OPENCL_VERSION

#include <vector>
#include <string>
#include <CL/cl.hpp>
#include <cstddef>
#include "Device.h"
#include "Context.h"
#include <memory>

namespace MCL
{

class CommandQueue
{
public:
    ~CommandQueue() = default;
    CommandQueue(const CommandQueue&) = default;
    CommandQueue(CommandQueue&& src) = default;

    CommandQueue& operator = (const CommandQueue&) = default;
    CommandQueue& operator = (CommandQueue&& src) = default;

    /************************************************************
    * @brief create a command queue on the device and context
    * @param[in] dev the device
    * @param[in] ctx the context
    * @param[in] properties the properties
    * @return the command queue
    ************************************************************/
    static CommandQueue create(const Device& dev, 
                                const Context& ctx, 
                                const cl_queue_properties *properties = nullptr) noexcept;

    cl_command_queue id() const noexcept;

private:
    CommandQueue(cl_command_queue id) noexcept;

    std::shared_ptr<cl_command_queue> m_id;
};

}; // namespace MCL

#endif // MCL_COMMANDQUEUE_H