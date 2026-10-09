#ifndef CAMERA_PREVIEW_CORE_FRAME_HPP
#define CAMERA_PREVIEW_CORE_FRAME_HPP

#include <chrono>
#include <cstdint>
#include <opencv2/core.hpp>

struct Frame
{
    cv::Mat image;
    std::chrono::steady_clock::time_point timestamp;
    std::uint64_t sequence{0};
};

#endif // CAMERA_PREVIEW_CORE_FRAME_HPP
