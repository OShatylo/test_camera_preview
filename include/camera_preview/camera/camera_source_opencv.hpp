#ifndef CAMERA_PREVIEW_CAMERA_CAMERA_SOURCE_OPENCV_HPP
#define CAMERA_PREVIEW_CAMERA_CAMERA_SOURCE_OPENCV_HPP

#include "interfaces/camera_source.hpp"

#include <cstdint>
#include <opencv2/videoio.hpp>

namespace camera_source
{

class Camera_source_opencv final : public Camera_source
{
  public:
    bool open(const std::string &camera_device, std::uint32_t width, std::uint32_t height, std::uint32_t fps) override;
    bool read(Frame &frame) override;

  private:
    cv::VideoCapture camera_;
    std::uint64_t sequence_{0};
};

} // namespace camera_source

#endif // CAMERA_PREVIEW_CAMERA_CAMERA_SOURCE_OPENCV_HPP
