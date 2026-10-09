#ifndef CAMERA_PREVIEW_INTERFACES_CAMERA_SOURCE_HPP
#define CAMERA_PREVIEW_INTERFACES_CAMERA_SOURCE_HPP

#include "core/frame.hpp"

#include <cstdint>
#include <string>

namespace camera_source
{

class Camera_source
{
  public:
    virtual ~Camera_source() = default;

    virtual bool open(const std::string &camera_device, std::uint32_t width, std::uint32_t height,
                      std::uint32_t fps) = 0;

    virtual bool read(Frame &frame) = 0;
};

} // namespace camera_source

#endif // CAMERA_PREVIEW_INTERFACES_CAMERA_SOURCE_HPP
