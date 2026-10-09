#ifndef CAMERA_PREVIEW_INTERFACES_DEVICE_VALIDATOR_HPP
#define CAMERA_PREVIEW_INTERFACES_DEVICE_VALIDATOR_HPP

#include <cstdint>
#include <string>

namespace device_validator
{

class Device_validator
{
  public:
    virtual ~Device_validator() = default;

    virtual bool is_valid(const std::string &camera_device, std::uint32_t width, std::uint32_t height,
                          std::uint32_t fps) const = 0;
};

} // namespace device_validator

#endif // CAMERA_PREVIEW_INTERFACES_DEVICE_VALIDATOR_HPP
