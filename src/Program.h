#ifndef MCL_PROGRAME_H
#define MCL_PROGRAME_H

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

namespace MCL
{

class Program
{
public:
    using Deleter = std::function<void()>;

    ~Program() = default;
    Program(const Program&) = default;
    Program(Program&& src) = default;

    Program& operator = (const Program&) = default;
    Program& operator = (Program&& src) = default;

    static const Program invalid;

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
    std::shared_ptr<cl_program> m_id;
    Deleter m_deleter;
};

}; // namespace MCL

#endif // MCL_PROGRAME_H