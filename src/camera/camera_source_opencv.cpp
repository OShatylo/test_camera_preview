#include "camera/camera_source_opencv.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

namespace
{

constexpr std::uint32_t byte_mask{0xFF};

constexpr int second_byte_shift{8};
constexpr int third_byte_shift{16};
constexpr int fourth_byte_shift{24};

std::string four_character_code_to_string(std::uint32_t four_character_code)
{
    if (four_character_code == 0)
    {
        return "unknown";
    }

    const char format[] = {static_cast<char>(four_character_code & byte_mask),
                           static_cast<char>((four_character_code >> second_byte_shift) & byte_mask),
                           static_cast<char>((four_character_code >> third_byte_shift) & byte_mask),
                           static_cast<char>((four_character_code >> fourth_byte_shift) & byte_mask)};

    return {format, 4};
}

} // namespace

namespace camera_source
{

bool Camera_source_opencv::open(const std::string &camera_device, std::uint32_t width, std::uint32_t height,
                                std::uint32_t fps)
{
    if (!camera_.open(camera_device, cv::CAP_V4L2))
    {
        std::cerr << "Failed to open camera: " << camera_device << "\n";

        return false;
    }

    if (width > 0)
    {
        camera_.set(cv::CAP_PROP_FRAME_WIDTH, static_cast<double>(width));
    }

    if (height > 0)
    {
        camera_.set(cv::CAP_PROP_FRAME_HEIGHT, static_cast<double>(height));
    }

    if (fps > 0)
    {
        camera_.set(cv::CAP_PROP_FPS, static_cast<double>(fps));
    }

    const auto current_width{static_cast<std::uint32_t>(camera_.get(cv::CAP_PROP_FRAME_WIDTH))};

    const auto current_height{static_cast<std::uint32_t>(camera_.get(cv::CAP_PROP_FRAME_HEIGHT))};

    const auto current_fps{camera_.get(cv::CAP_PROP_FPS)};

    const auto four_character_code{static_cast<std::uint32_t>(camera_.get(cv::CAP_PROP_FOURCC))};

    std::cout << "Camera: " << camera_device << "\n"
              << "Resolution: " << current_width << "x" << current_height << "\n"
              << "Frame format: " << four_character_code_to_string(four_character_code) << "\n";

    if (current_fps > 0.0)
    {
        std::cout << "FPS: " << current_fps << "\n";
    }
    else
    {
        std::cout << "FPS unavailable\n";
    }

    return true;
}

bool Camera_source_opencv::read(Frame &frame)
{
    if (!camera_.read(frame.image) || frame.image.empty())
    {
        return false;
    }

    frame.timestamp = std::chrono::steady_clock::now();

    frame.sequence = sequence_++;

    return true;
}

} // namespace camera_source
