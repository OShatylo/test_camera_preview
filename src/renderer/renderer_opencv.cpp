#include "renderer/renderer_opencv.hpp"

#include <iomanip>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>
#include <string>

namespace
{

constexpr int key_mask{0xFF};
constexpr int key_escape{27};

constexpr double font_scale{0.6};
constexpr int font_thickness{2};

const std::string window_name{"Camera Preview"};

bool is_exit_key(int key)
{
    return (key == key_escape) || (key == 'q') || (key == 'Q');
}

std::string create_overlay(const renderer::Render_statistics &statistics)
{
    std::ostringstream stream;

    stream << std::fixed << std::setprecision(1) << "FPS: " << statistics.fps << " | Age: " << statistics.frame_age_ms
           << " ms" << " | Dropped: " << statistics.dropped_frames;

    return stream.str();
}

} // namespace

namespace renderer
{

bool Renderer_opencv::render(Frame &frame, const Render_statistics &statistics)
{
    cv::putText(frame.image, create_overlay(statistics), cv::Point{20, 30}, cv::FONT_HERSHEY_SIMPLEX, font_scale,
                cv::Scalar{0, 255, 0}, font_thickness);

    cv::imshow(window_name, frame.image);

    const int key{cv::waitKey(1) & key_mask};

    return !is_exit_key(key);
}

void Renderer_opencv::close()
{
    cv::destroyAllWindows();
}

} // namespace renderer
