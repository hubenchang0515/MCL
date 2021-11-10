#ifndef MCL_BUFFER_H
#define MCL_BUFFER_H

#ifndef CL_HPP_TARGET_OPENCL_VERSION 
#define CL_HPP_TARGET_OPENCL_VERSION 210
#endif // CL_HPP_TARGET_OPENCL_VERSION

#include <vector>
#include <string>
#include <CL/cl2.hpp>
#include <cstddef>
#include "Device.h"
#include "Context.h"
#include "CommandQueue.h"

namespace MCL
{

class Buffer
{
public:
    ~Buffer() noexcept;
    Buffer(const Buffer&) = delete;
    Buffer(Buffer&& src) noexcept;

    static const Buffer invalid;

    /************************************************************
    * @brief create a buffer on context
    * @param[in] ctx the context
    * @param[in] size the size of buffer in bytes
    * @param[in] flags read-write flags
    * @param[in] data the init data atleast the size bytes 
    * @return the command queue
    ************************************************************/
    static Buffer create(const Context& ctx, 
                            size_t size, 
                            cl_mem_flags flags=CL_MEM_READ_WRITE,
                            void* data=nullptr) noexcept;

    cl_mem mem() const noexcept;
    size_t size() const noexcept;

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
    cl_int write(const CommandQueue& cmd, 
                void* data, 
                size_t size, 
                size_t offset=0,
                bool blocking=true,
                cl_uint numEventsInWaitList=0, 
                const cl_event* eventWaitList=nullptr, 
                cl_event* event=nullptr) const noexcept;

    /************************************************************
    * @brief enqueue a command to read buffer
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
    cl_int read(const CommandQueue& cmd, 
                void* data, 
                size_t size, 
                size_t offset=0,
                bool blocking=true,
                cl_uint numEventsInWaitList=0, 
                const cl_event* eventWaitList=nullptr, 
                cl_event* event=nullptr) const noexcept;

private:
    Buffer(cl_mem mem, size_t size) noexcept;
    cl_mem m_mem;
    size_t m_size;
};

}; // namespace MCL

#endif // MCL_BUFFER_H