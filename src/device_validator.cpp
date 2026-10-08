#include "device_validator.hpp"
#include "helpers.hpp"
#include <cmath>
#include <cstdint>
#include <fcntl.h>
#include <iostream>
#include <linux/videodev2.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

namespace
{

bool is_resolution_matching(std::uint32_t requested_width, std::uint32_t requested_height, std::uint32_t current_width,
                            std::uint32_t current_height)
{
    const bool width_matches{(requested_width == 0) || (requested_width == current_width)};

    const bool height_matches{(requested_height == 0) || (requested_height == current_height)};

    return width_matches && height_matches;
}

bool is_frame_interval_matching(const v4l2_frmivalenum &frame_interval, std::uint32_t requested_fps)
{
    if (frame_interval.type != V4L2_FRMIVAL_TYPE_DISCRETE)
    {
        return false;
    }

    if (frame_interval.discrete.numerator == 0)
    {
        return false;
    }

    const double current_fps{static_cast<double>(frame_interval.discrete.denominator) /
                             frame_interval.discrete.numerator};

    const auto rounded_fps{static_cast<std::uint32_t>(std::lround(current_fps))};

    return rounded_fps == requested_fps;
}

bool is_fps_supported(int fd, std::uint32_t pixel_format, std::uint32_t width, std::uint32_t height,
                      std::uint32_t requested_fps)
{
    v4l2_frmivalenum frame_interval{};

    frame_interval.pixel_format = pixel_format;
    frame_interval.width = width;
    frame_interval.height = height;

    for (frame_interval.index = 0; ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &frame_interval) == 0; ++frame_interval.index)
    {
        if (is_frame_interval_matching(frame_interval, requested_fps))
        {
            return true;
        }
    }

    return false;
}

bool is_frame_size_supported(int fd, std::uint32_t pixel_format, const v4l2_frmsizeenum &frame_size,
                             std::uint32_t requested_width, std::uint32_t requested_height, std::uint32_t requested_fps)
{
    if (frame_size.type != V4L2_FRMSIZE_TYPE_DISCRETE)
    {
        return false;
    }

    const auto current_width{frame_size.discrete.width};

    const auto current_height{frame_size.discrete.height};

    if (!is_resolution_matching(requested_width, requested_height, current_width, current_height))
    {
        return false;
    }

    if (requested_fps == 0)
    {
        return true;
    }

    return is_fps_supported(fd, pixel_format, current_width, current_height, requested_fps);
}

bool is_format_supported(int fd, std::uint32_t pixel_format, std::uint32_t requested_width,
                         std::uint32_t requested_height, std::uint32_t requested_fps)
{
    v4l2_frmsizeenum frame_size{};
    frame_size.pixel_format = pixel_format;

    for (frame_size.index = 0; ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frame_size) == 0; ++frame_size.index)
    {
        if (is_frame_size_supported(fd, pixel_format, frame_size, requested_width, requested_height, requested_fps))
        {
            return true;
        }
    }

    return false;
}

bool validate_camera_parameters(int fd, std::uint32_t requested_width, std::uint32_t requested_height,
                                std::uint32_t requested_fps)
{
    v4l2_fmtdesc format{};
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    for (format.index = 0; ioctl(fd, VIDIOC_ENUM_FMT, &format) == 0; ++format.index)
    {
        if (is_format_supported(fd, format.pixelformat, requested_width, requested_height, requested_fps))
        {
            return true;
        }
    }

    return false;
}

bool validate_device(const std::string &camera_device, std::uint32_t width, std::uint32_t height, std::uint32_t fps)
{
    RAII_FD fd{open(camera_device.c_str(), O_RDWR | O_NONBLOCK)};

    if (fd.get() < 0)
    {
        std::cerr << "Cannot open device: " << camera_device << '\n';

        return false;
    }

    v4l2_capability capability{};

    if (ioctl(fd.get(), VIDIOC_QUERYCAP, &capability) < 0)
    {
        std::cerr << "Cannot query device capabilities: " << camera_device << '\n';

        return false;
    }

    return validate_camera_parameters(fd.get(), width, height, fps);
}

} // namespace

namespace device_validator
{

Device_validator::Device_validator(const std::string &camera_device, std::uint32_t width, std::uint32_t height,
                                   std::uint32_t fps)
    : camera_device_{camera_device}, width_{width}, height_{height}, fps_{fps},
      is_valid_{validate_device(camera_device_, width_, height_, fps_)}
{
}

bool Device_validator::is_valid() const
{
    return is_valid_;
}

} // namespace device_validator
