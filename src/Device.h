#ifndef MCL_DEVICE_H
#define MCL_DEVICE_H

#ifndef CL_HPP_TARGET_OPENCL_VERSION 
#define CL_HPP_TARGET_OPENCL_VERSION 210
#endif // CL_HPP_TARGET_OPENCL_VERSION

#include <vector>
#include <string>
#include <CL/cl2.hpp>
#include <cstddef>

namespace MCL
{

class Platform;

class Device
{
public:
    ~Device() = default;
    Device(const Device&) = default;
    Device(Device&&) = default;

    Device& operator = (const Device&) = default;
    Device& operator = (Device&& src) = default;

    static const Device invalid;

    /************************************************************
    * @brief get all devices on the platform with type 
    * @param[in] plat the platform
    * @param[in] dtype device type
    * @return the devices
    ************************************************************/
    static std::vector<Device> get(const Platform& plat, cl_device_type dtype) noexcept;

    cl_device_id id() const noexcept;
    std::string info(cl_device_info iname) const noexcept;

    /* Partiallyordered set */
    bool operator < (const Device& another) const noexcept;
    bool operator > (const Device& another) const noexcept;
    bool operator == (const Device& another) const noexcept;
    bool operator != (const Device& another) const noexcept;
    bool operator <= (const Device& another) const noexcept;
    bool operator >= (const Device& another) const noexcept;

private:
    cl_device_id m_id;
    Device(cl_device_id id) noexcept;
};

}; // namespace MCL

#endif // MCL_DEVICE_H