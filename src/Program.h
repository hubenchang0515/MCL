#ifndef MCL_PROGRAME_H
#define MCL_PROGRAME_H

#ifndef CL_TARGET_OPENCL_VERSION 
#define CL_TARGET_OPENCL_VERSION 220
#endif // CL_TARGET_OPENCL_VERSION

#include <vector>
#include <string>
#include <CL/cl.hpp>
#include <cstddef>
#include <functional>
#include "Device.h"
#include "Context.h"
#include "CommandQueue.h"

namespace MCL
{

class Program
{
public:
    using Deleter = std::function<void()>;

    ~Program() noexcept;
    Program(const Program&) = delete;
    Program(Program&& src) noexcept;

    static Program load(const Context& ctx, const char* file) noexcept;
    static Program load(const Context& ctx, const char* code, size_t size, Deleter deleter=nullptr) noexcept;

    cl_program id() const noexcept;
    size_t size() const noexcept;
    void print() const noexcept;
    bool build(const Device& dev) const noexcept;
    std::string info(const Device& dev) const noexcept;

private:
    Program(const char* code, size_t size, cl_program id, Deleter deleter=nullptr) noexcept;

    const char* m_code;
    size_t m_size;
    cl_program m_id;
    Deleter m_deleter;
};

}; // namespace MCL

#endif // MCL_PROGRAME_H