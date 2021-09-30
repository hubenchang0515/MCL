#include "Buffer.h"

namespace MCL
{

/************************************************************
* @brief create a buffer on context
* @param[in] ctx the context
* @param[in] size the size of buffer on bytes
* @param[in] flags read-write flags
* @param[in] data the init data atleast the size bytes 
* @return the command queue
************************************************************/
Buffer Buffer::create(const Context& ctx, 
                        size_t size, 
                        cl_mem_flags flags,
                        void* data) noexcept
{
    cl_int err;
    cl_mem m = clCreateBuffer(ctx.id(), flags, size, data, &err);
    if (m == nullptr || err != CL_SUCCESS)
    {
        return Buffer(nullptr, 0);
    }
    return Buffer(m, size);
}

Buffer::~Buffer() noexcept
{
    if (m_mem == nullptr)
        return;

    clReleaseMemObject(m_mem);
    m_mem = nullptr;
}

Buffer::Buffer(cl_mem mem, size_t size) noexcept :
    m_mem(mem),
    m_size(size)
{

}

Buffer::Buffer(Buffer&& src) noexcept :
    m_mem(src.m_mem)
{
    src.m_mem = nullptr;
}

cl_mem Buffer::mem() const noexcept
{
    return m_mem;
}

size_t Buffer::size() const noexcept
{
    return m_size;
}

/************************************************************
* @brief enqueue a command to write buffer
*        see https://www.khronos.org/registry/OpenCL/sdk/2.2/docs/man/html/clEnqueueWriteBuffer.html
* @param[in] cmd the command queue
* @param[in] data the data
* @param[in] size the size of data in bytes
* @param[in] offset the offset in data
* @param[in] blocking is blocking
* @param[in] numEventsInWaitList number of events in wait list
* @param[in] eventWaitList event wait list
* @param[in] event event
* @return the command queue
************************************************************/
cl_int Buffer::write(const CommandQueue& cmd, 
                        void* data, 
                        size_t size, 
                        size_t offset,
                        bool blocking,
                        cl_uint numEventsInWaitList, 
                        const cl_event* eventWaitList, 
                        cl_event* event) const noexcept
{
    if (size == 0)
    {
        size = m_size;
    }

    return clEnqueueWriteBuffer(cmd.id(), m_mem, blocking, offset, size, data, 
                                numEventsInWaitList, eventWaitList, event);
}

/************************************************************
* @brief enqueue a command to write buffer
*        see https://www.khronos.org/registry/OpenCL/sdk/2.2/docs/man/html/clEnqueueWriteBuffer.html
* @param[in] cmd the command queue
* @param[in] data the data
* @param[in] size the size of data in bytes
* @param[in] offset the offset in data
* @param[in] blocking is blocking
* @param[in] numEventsInWaitList number of events in wait list
* @param[in] eventWaitList event wait list
* @param[in] event event
* @return the command queue
************************************************************/
cl_int Buffer::read(const CommandQueue& cmd, 
                        void* data, 
                        size_t size, 
                        size_t offset,
                        bool blocking,
                        cl_uint numEventsInWaitList, 
                        const cl_event* eventWaitList, 
                        cl_event* event) const noexcept
{
    if (size == 0)
    {
        size = m_size;
    }

    return clEnqueueReadBuffer(cmd.id(), m_mem, blocking, offset, size, data, 
                                numEventsInWaitList, eventWaitList, event);
}

}; // namespace MCL