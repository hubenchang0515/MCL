#ifndef MCL_PLATFORM_H
#define MCL_PLATFORM_H

#ifndef CL_HPP_TARGET_OPENCL_VERSION 
#define CL_HPP_TARGET_OPENCL_VERSION 210
#endif // CL_HPP_TARGET_OPENCL_VERSION

#include <vector>
#include <string>
#include <CL/cl2.hpp>

namespace MCL
{

class Platform
{
public:
    ~Platform() = default;
    Platform(const Platform& plat) = default;
    Platform(Platform&& plat) = default;

    Platform& operator = (const Platform&) = default;
    Platform& operator = (Platform&& src) = default;

    static const Platform invalid;

    /************************************************************
    * @brief get all platforms 
    * @return the platforms
    ************************************************************/
    static std::vector<Platform> get() noexcept;

    cl_platform_id id() const noexcept;
    std::string info(cl_platform_info iname) const noexcept;
    std::string profile() const noexcept;
    std::string version() const noexcept;
    std::string name() const noexcept;
    std::string vendor() const noexcept;
    std::string extensions() const noexcept;

    /* Partiallyordered set */
    bool operator < (const Platform& another) const noexcept;
    bool operator > (const Platform& another) const noexcept;
    bool operator == (const Platform& another) const noexcept;
    bool operator != (const Platform& another) const noexcept;
    bool operator <= (const Platform& another) const noexcept;
    bool operator >= (const Platform& another) const noexcept;


private:
    cl_platform_id m_id;
    Platform(cl_platform_id id) noexcept;

};

}; // namespace MCL

#endif // MCL_PLATFORM_H