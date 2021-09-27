#include "Platform.h"

namespace MCL
{

/************************************************************
* @brief get platforms no more then n
* @param[in] n the max 
* @return the platforms
************************************************************/
std::vector<Platform> Platform::get() noexcept
{
    std::vector<Platform> platforms;
    cl_uint n = 0;
    cl_int err = clGetPlatformIDs(0, nullptr, &n);
    if (err != CL_SUCCESS)
    {
        return platforms;
    }

    cl_platform_id* pids = new cl_platform_id[n];
    clGetPlatformIDs(n, pids, nullptr);
    for (cl_uint i = 0; i < n; i++)
    {
        platforms.emplace_back(Platform(pids[i]));
    }
    delete[] pids;

    return platforms;
}

Platform::Platform(cl_platform_id id) noexcept :
    m_id(id)
{

}

cl_platform_id Platform::id() const noexcept
{
    return m_id;
}

std::string Platform::info(cl_platform_info iname) const noexcept
{
    size_t n = 0;
    cl_int err = clGetPlatformInfo(m_id, iname, 0, nullptr, &n); // n will include the '\0'
    if (err != CL_SUCCESS || n == 0)
    {
        return "";
    }

    char* name = new char[n];
    clGetPlatformInfo(m_id, iname, n, name, nullptr);
    std::string ret = name;
    delete[] name;

    return ret;
}

std::string Platform::profile() const noexcept
{
    return this->info(CL_PLATFORM_PROFILE);
}

std::string Platform::version() const noexcept
{
    return this->info(CL_PLATFORM_VERSION);
}


std::string Platform::name() const noexcept
{
    return this->info(CL_PLATFORM_NAME);
}

std::string Platform::vendor() const noexcept
{
    return this->info(CL_PLATFORM_VENDOR);
}

std::string Platform::extensions() const noexcept
{
    return this->info(CL_PLATFORM_EXTENSIONS);
}


}; // namespace MCL