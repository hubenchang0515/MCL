#include "Device.h"
#include "Platform.h"

namespace MCL
{

const Device Device::invalid = nullptr;

std::vector<Device> Device::get(const Platform& plat, cl_device_type dtype) noexcept
{
    std::vector<Device> devices;
    cl_uint n = 0;
    cl_int err = clGetDeviceIDs(plat.id(), dtype, 0, nullptr, &n);
    if (err != CL_SUCCESS)
    {
        return devices;
    }

    cl_device_id* devs = new cl_device_id[n];
    clGetDeviceIDs(plat.id(), dtype, n, devs, nullptr);
    for (cl_uint i = 0; i < n; i++)
    {
        devices.emplace_back(Device(devs[i]));
    }

    delete[] devs;
    return devices;
}

Device::Device(cl_device_id id) noexcept :
    m_id(id)
{

}

cl_device_id Device::id() const noexcept
{
    return m_id;
}

std::string Device::info(cl_device_info iname) const noexcept
{
    size_t n = 0;
    cl_int err = clGetDeviceInfo(m_id, iname, 0, nullptr, &n); // n will include the '\0'
    if (err != CL_SUCCESS || n == 0)
    {
        return "";
    }

    char* name = new char[n];
    clGetDeviceInfo(m_id, iname, n, name, nullptr);
    std::string ret = name;
    delete[] name;

    return ret;
}

bool Device::operator < (const Device& another) const noexcept
{
    return m_id < another.m_id;
}

bool Device::operator > (const Device& another) const noexcept
{
    return m_id > another.m_id;
}

bool Device::operator == (const Device& another) const noexcept
{
    return m_id == another.m_id;
}

bool Device::operator != (const Device& another) const noexcept
{
    return m_id != another.m_id;
}

bool Device::operator <= (const Device& another) const noexcept
{
    return m_id <= another.m_id;
}

bool Device::operator >= (const Device& another) const noexcept
{
    return m_id >= another.m_id;
}

}; // namespace MCL