#include "camera/device_validator_linux.hpp"

#include "utils/utils.hpp"
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

bool is_video_capture_device(const v4l2_capability &capability)
{
    const std::uint32_t capabilities{(capability.capabilities & V4L2_CAP_DEVICE_CAPS) ? capability.device_caps
                                                                                      : capability.capabilities};

    return (capabilities & V4L2_CAP_VIDEO_CAPTURE) != 0;
}

bool is_resolution_matching(std::uint32_t requested_width, std::uint32_t requested_height, std::uint32_t current_width,
                            std::uint32_t current_height)
{
    const bool width_matches{requested_width == 0 || requested_width == current_width};

    const bool height_matches{requested_height == 0 || requested_height == current_height};

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
    if (requested_fps == 0)
    {
        return true;
    }

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

bool is_frame_size_supported(int fd, std::uint32_t pixel_format, std::uint32_t requested_width,
                             std::uint32_t requested_height, std::uint32_t requested_fps)
{
    v4l2_frmsizeenum frame_size{};

    frame_size.pixel_format = pixel_format;

    for (frame_size.index = 0; ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frame_size) == 0; ++frame_size.index)
    {
        if (frame_size.type != V4L2_FRMSIZE_TYPE_DISCRETE)
        {
            continue;
        }

        const auto width{frame_size.discrete.width};

        const auto height{frame_size.discrete.height};

        if (!is_resolution_matching(requested_width, requested_height, width, height))
        {
            continue;
        }

        if (is_fps_supported(fd, pixel_format, width, height, requested_fps))
        {
            return true;
        }
    }

    return false;
}

bool is_format_supported(int fd, const v4l2_fmtdesc &format, std::uint32_t width, std::uint32_t height,
                         std::uint32_t fps)
{
    return is_frame_size_supported(fd, format.pixelformat, width, height, fps);
}

bool validate_camera_parameters(int fd, std::uint32_t width, std::uint32_t height, std::uint32_t fps)
{
    v4l2_fmtdesc format{};

    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    for (format.index = 0; ioctl(fd, VIDIOC_ENUM_FMT, &format) == 0; ++format.index)
    {
        if (is_format_supported(fd, format, width, height, fps))
        {
            return true;
        }
    }

    return false;
}

bool validate_device(const std::string &camera_device, std::uint32_t width, std::uint32_t height, std::uint32_t fps)
{
    utils::RAII_FD fd{open(camera_device.c_str(), O_RDWR | O_NONBLOCK)};

    if (fd.get() < 0)
    {
        std::cerr << "Failed to open camera device: " << camera_device << '\n';

        return false;
    }

    v4l2_capability capability{};

    if (ioctl(fd.get(), VIDIOC_QUERYCAP, &capability) < 0)
    {
        std::cerr << "Failed to query camera capabilities: " << camera_device << '\n';

        return false;
    }

    if (!is_video_capture_device(capability))
    {
        std::cerr << "Device does not support video capture: " << camera_device << '\n';

        return false;
    }

    return validate_camera_parameters(fd.get(), width, height, fps);
}

} // namespace

namespace device_validator
{

bool Device_validator_linux::is_valid(const std::string &camera_device, std::uint32_t width, std::uint32_t height,
                                      std::uint32_t fps) const
{
    return validate_device(camera_device, width, height, fps);
}

} // namespace device_validator
