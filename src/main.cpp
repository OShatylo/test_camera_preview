#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

namespace
{
constexpr int byte_mask = 0xFF;
constexpr int second_byte_shift = 8;
constexpr int third_byte_shift = 16;
constexpr int fourth_byte_shift = 24;
constexpr int key_escape = 27;
} // namespace

std::string four_character_code_to_string(std::uint32_t four_character_code)
{
    if (four_character_code == 0U)
    {
        return "unknown";
    }

    const char format[] = {static_cast<char>(four_character_code & byte_mask),
                           static_cast<char>((four_character_code >> second_byte_shift) & byte_mask),
                           static_cast<char>((four_character_code >> third_byte_shift) & byte_mask),
                           static_cast<char>((four_character_code >> fourth_byte_shift) & byte_mask)};

    return {format, 4};
}

int main(int argc, char *argv[])
{
    constexpr int camera_device_idx{0};

    cv::VideoCapture camera(camera_device_idx, cv::CAP_V4L);

    if (!camera.isOpened())
    {
        std::cerr << "Failed to open camera /dev/video" << camera_device_idx << '\n';

        return EXIT_FAILURE;
    }

    const auto width{static_cast<std::uint32_t>(camera.get(cv::CAP_PROP_FRAME_WIDTH))};

    const auto height{static_cast<std::uint32_t>(camera.get(cv::CAP_PROP_FRAME_HEIGHT))};

    const auto driver_fps{camera.get(cv::CAP_PROP_FPS)};

    const auto four_character_code{static_cast<std::uint32_t>(camera.get(cv::CAP_PROP_FOURCC))};

    std::cout << "Camera: /dev/video" << camera_device_idx << '\n';

    std::cout << "Resolution: " << width << 'x' << height << '\n';

    std::cout << "Frame format: " << four_character_code_to_string(four_character_code) << '\n';

    if (driver_fps > 0.0)
    {
        std::cout << "FPS: " << driver_fps << '\n';
    }
    else
    {
        std::cout << "FPS unavailable...\n";
    }

    cv::Mat frame;

    std::size_t frame_counter{0};

    auto fps_start{std::chrono::steady_clock::now()};

    while (true)
    {
        if (!camera.read(frame) || frame.empty())
        {
            std::cerr << "Failed to read frame\n";
            break;
        }

        ++frame_counter;

        const auto now = std::chrono::steady_clock::now();

        const auto elapsed = std::chrono::duration<double>(now - fps_start).count();

        if (elapsed >= 1.0)
        {
            const auto real_fps = static_cast<double>(frame_counter) / elapsed;

            std::cout << "Measured FPS: " << std::fixed << std::setprecision(1) << real_fps << '\n';

            frame_counter = 0;
            fps_start = now;
        }

        cv::imshow("Camera preview", frame);

        const auto key = cv::waitKey(1) & byte_mask;

        if ((key == key_escape) || (key == 'q') || (key == 'Q'))
        {
            break;
        }
    }

    camera.release();
    cv::destroyAllWindows();

    return EXIT_SUCCESS;
}
