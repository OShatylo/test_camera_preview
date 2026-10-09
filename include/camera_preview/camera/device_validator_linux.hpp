#ifndef CAMERA_PREVIEW_CAMERA_V4L2_DEVICE_VALIDATOR_HPP
#define CAMERA_PREVIEW_CAMERA_V4L2_DEVICE_VALIDATOR_HPP

#include "interfaces/device_validator.hpp"

namespace device_validator
{

class Device_validator_linux final : public Device_validator
{
  public:
    Device_validator_linux() = default;

    bool is_valid(const std::string &camera_device, std::uint32_t width, std::uint32_t height,
                  std::uint32_t fps) const override;
};

} // namespace device_validator

#endif // CAMERA_PREVIEW_CAMERA_V4L2_DEVICE_VALIDATOR_HPP
