#include "Program.h"

namespace MCL
{

Program Program::load(const Context& ctx, const char* file) noexcept
{
    FILE* fp = fopen(file, "rb");
    if (fp == nullptr)
    {
        return Program(nullptr, 0, nullptr);
    }

    static const size_t step = 256;
    ptrdiff_t offset = 0;
    size_t n = 0;
    size_t size = 0;
    char* code =nullptr;

    do
    {
        void* newCode = realloc(code, size + step);
        if (newCode == nullptr)
        {
            if (code != nullptr)
                free(code);
            fclose(fp);
            return Program(nullptr, 0, nullptr);
        }

        code = static_cast<char*>(newCode);
        n = fread(code + offset, 1, step, fp);
        offset += step;
        size += n;
    }while(n == step);

    cl_int err;
    cl_program id = clCreateProgramWithSource(ctx.id(), 1, const_cast<const char**>(&code), &size, &err);
    if (id == nullptr || err != CL_SUCCESS)
    {
        free(code);
        fclose(fp);
        return Program(nullptr, 0, nullptr);
    }

    Program ret{const_cast<const char*>(code), size, id, [code](){free(code);}};
    fclose(fp);
    return ret;
}

Program Program::load(const Context& ctx, const char* code, size_t size, Deleter deleter) noexcept
{
    cl_int err;
    cl_program id = clCreateProgramWithSource(ctx.id(), 1, const_cast<const char**>(&code), &size, &err);
    if (id == nullptr || err != CL_SUCCESS)
    {
        deleter();
        return Program(nullptr, 0, nullptr);
    }

    return Program{const_cast<const char*>(code), size, id, deleter};
}

Program::Program(const char* code, size_t size, cl_program id, Deleter deleter) noexcept :
    m_code(code),
    m_size(size),
    m_id(new cl_program, [](cl_program* id){clReleaseProgram(*id); delete id;}),
    m_deleter(deleter)
{
    *m_id = id;
}

Program::Program(Program&& src) noexcept :
    m_code(src.m_code),
    m_size(src.m_size),
    m_id(src.m_id),
    m_deleter(src.m_deleter)
{
    src.m_code = nullptr;
    src.m_size = 0;
    src.m_id = nullptr;
    src.m_deleter = nullptr;
}

cl_program Program::id() const noexcept
{
    return *m_id;
}

size_t Program::size() const noexcept
{
    return m_size;
}

void Program::print() const noexcept
{
    printf("%.*s\n", static_cast<int>(m_size), m_code);
}

bool Program::build(const Device& dev) const noexcept
{
    cl_device_id devId = dev.id();
    cl_int err = clBuildProgram(*m_id, 1, &devId, nullptr, nullptr, nullptr);
    return err == CL_SUCCESS;
}

std::string Program::info(const Device& dev) const noexcept
{
    cl_device_id devId = dev.id();
    size_t len = 0;
    clGetProgramBuildInfo(*m_id, devId, CL_PROGRAM_BUILD_LOG, 0, nullptr, &len);
    char* msg = new char[len];
    clGetProgramBuildInfo(*m_id, devId, CL_PROGRAM_BUILD_LOG, len, msg, 0);
    std::string ret{msg, len};
    delete[] msg;
    return ret;
}

}; // namespace MCL