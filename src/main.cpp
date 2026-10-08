#include "commandline_arguments_parser.hpp"
#include "device_validator.hpp"

#include <chrono>
#include <iomanip>
#include <opencv2/opencv.hpp>

namespace
{
constexpr std::uint32_t byte_mask{0xFF};
constexpr int second_byte_shift{8};
constexpr int third_byte_shift{16};
constexpr int fourth_byte_shift{24};

constexpr int key_mask{0xFF};
constexpr int key_escape{27};
constexpr double fps_measurement_interval_sec{1.0};

} // namespace

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

int main(int argc, char *argv[])
{
    Commandline_arguments::Commandline_arguments_parser arguments_parser{argc, argv};

    if (!arguments_parser.is_valid())
    {
        return EXIT_FAILURE;
    }

    const auto &args{arguments_parser.get_args()};

    device_validator::Device_validator device_validator{args.camera_device, args.width, args.height, args.fps};

    if (!device_validator.is_valid())
    {
        std::cerr << "Requested camera configuration is not supported\n";
        return EXIT_FAILURE;
    }

    cv::VideoCapture camera{args.camera_device, cv::CAP_V4L2};

    if (!camera.isOpened())
    {
        std::cerr << "Failed to open camera: " << args.camera_device << '\n';

        return EXIT_FAILURE;
    }

    if (args.width > 0)
    {
        camera.set(cv::CAP_PROP_FRAME_WIDTH, static_cast<double>(args.width));
    }

    if (args.height > 0)
    {
        camera.set(cv::CAP_PROP_FRAME_HEIGHT, static_cast<double>(args.height));
    }

    if (args.fps > 0)
    {
        camera.set(cv::CAP_PROP_FPS, static_cast<double>(args.fps));
    }

    const auto width{static_cast<std::uint32_t>(camera.get(cv::CAP_PROP_FRAME_WIDTH))};

    const auto height{static_cast<std::uint32_t>(camera.get(cv::CAP_PROP_FRAME_HEIGHT))};

    const auto driver_fps{camera.get(cv::CAP_PROP_FPS)};

    const auto four_character_code{static_cast<std::uint32_t>(camera.get(cv::CAP_PROP_FOURCC))};

    std::cout << "Camera: " << args.camera_device << '\n';

    std::cout << "Resolution: " << width << 'x' << height << '\n';

    std::cout << "Frame format: " << four_character_code_to_string(four_character_code) << '\n';

    if (driver_fps > 0.0)
    {
        std::cout << "FPS: " << driver_fps << '\n';
    }
    else
    {
        std::cout << "FPS unavailable\n";
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

        const auto now{std::chrono::steady_clock::now()};

        const auto elapsed{std::chrono::duration<double>(now - fps_start).count()};

        if (elapsed >= fps_measurement_interval_sec)
        {
            const auto real_fps{static_cast<double>(frame_counter) / elapsed};

            std::cout << "Measured FPS: " << std::fixed << std::setprecision(1) << real_fps << '\n';

            frame_counter = 0;
            fps_start = now;
        }

        cv::imshow("Camera Preview", frame);

        const auto key{cv::waitKey(1) & key_mask};

        if ((key == key_escape) || (key == 'q') || (key == 'Q'))
        {
            break;
        }
    }

    camera.release();
    cv::destroyAllWindows();

    return EXIT_SUCCESS;
}
