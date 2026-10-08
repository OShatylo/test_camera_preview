#ifndef DEVICE_VALIDATOR_HPP
#define DEVICE_VALIDATOR_HPP

#include <cstdint>
#include <string>

namespace device_validator
{

class Device_validator
{
  public:
    Device_validator(const std::string &camera_device, std::uint32_t width, std::uint32_t height, std::uint32_t fps);

    bool is_valid() const;

  private:
    std::string camera_device_;

    std::uint32_t width_{0};
    std::uint32_t height_{0};
    std::uint32_t fps_{0};

    bool is_valid_{false};
};
} // namespace device_validator

#endif // DEVICE_VALIDATOR
