#include "CommandQueue.h"

namespace MCL
{

const CommandQueue CommandQueue::invalid{nullptr};

CommandQueue CommandQueue::create(const Device& dev, 
                                    const Context& ctx, 
                                    const cl_queue_properties *properties) noexcept
{
    cl_int err;
    cl_command_queue cmd = clCreateCommandQueueWithProperties(ctx.id(), dev.id(), properties, &err);
    return CommandQueue(cmd);
}

CommandQueue::~CommandQueue() noexcept
{
    if (m_id == nullptr)
        return;

    clReleaseCommandQueue(m_id);
    m_id = nullptr;
}

CommandQueue::CommandQueue(cl_command_queue id) noexcept :
    m_id(id)
{

}

CommandQueue::CommandQueue(CommandQueue&& src) noexcept :
    m_id(src.m_id)
{
    src.m_id = nullptr;
}

cl_command_queue CommandQueue::id() const noexcept
{
    return m_id;
}

bool CommandQueue::operator < (const CommandQueue& another) const noexcept
{
    return m_id < another.m_id;
}

bool CommandQueue::operator > (const CommandQueue& another) const noexcept
{
    return m_id > another.m_id;
}

bool CommandQueue::operator == (const CommandQueue& another) const noexcept
{
    return m_id == another.m_id;
}

bool CommandQueue::operator != (const CommandQueue& another) const noexcept
{
    return m_id != another.m_id;
}

bool CommandQueue::operator <= (const CommandQueue& another) const noexcept
{
    return m_id <= another.m_id;
}

bool CommandQueue::operator >= (const CommandQueue& another) const noexcept
{
    return m_id >= another.m_id;
}

}; // namespace MCL