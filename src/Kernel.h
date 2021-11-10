#ifndef MCL_KERNEL_H
#define MCL_KERNEL_H

#ifndef CL_HPP_TARGET_OPENCL_VERSION 
#define CL_HPP_TARGET_OPENCL_VERSION 210
#endif // CL_HPP_TARGET_OPENCL_VERSION

#include <vector>
#include <string>
#include <CL/cl2.hpp>
#include <cstddef>
#include <functional>
#include "Device.h"
#include "Context.h"
#include "CommandQueue.h"
#include "Program.h"
#include "Buffer.h"
#include <memory>

namespace MCL
{

class Kernel
{
public:
    ~Kernel() = default;
    Kernel(const Kernel&) = default;
    Kernel(Kernel&& src) = default;

    Kernel& operator = (const Kernel&) = default;
    Kernel& operator = (Kernel&& src) = default;

    static const Kernel invalid;

    static Kernel create(const Program& program, const char* name) noexcept;

    cl_kernel id() const noexcept;
    cl_int setArg(cl_uint index, const Buffer& data) const noexcept;
    cl_int invoke(const CommandQueue& cmd, size_t globalItems, size_t localItems) const noexcept;

private:
    Kernel(cl_kernel id);

    std::shared_ptr<cl_kernel> m_id;

};

}; // namespace MCL

#endif // MCL_KERNEL_H