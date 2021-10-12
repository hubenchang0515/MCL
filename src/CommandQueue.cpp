#include "CommandQueue.h"

namespace MCL
{

CommandQueue CommandQueue::create(const Device& dev, 
                                    const Context& ctx, 
                                    const cl_queue_properties *properties) noexcept
{
    cl_int err;
    cl_command_queue cmd = clCreateCommandQueueWithProperties(ctx.id(), dev.id(), properties, &err);
    return CommandQueue(cmd);
}

CommandQueue::CommandQueue(cl_command_queue id) noexcept :
    m_id(new cl_command_queue, [](cl_command_queue* id){clReleaseCommandQueue(*id); delete id;})
{
    *m_id = id;
}

cl_command_queue CommandQueue::id() const noexcept
{
    return *m_id;
}

}; // namespace MCL