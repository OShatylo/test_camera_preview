#include "application/application.hpp"

#include "core/latest_frame_buffer.hpp"

#include <atomic>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>

namespace
{

constexpr double fps_measurement_interval_sec{1.0};

void capture_loop(std::shared_ptr<camera_source::Camera_source> camera_source, Latest_frame_buffer &frame_buffer,
                  std::atomic<bool> &stop_requested, std::atomic<bool> &capture_failed)
{
    while (!stop_requested.load())
    {
        Frame frame;

        if (!camera_source->read(frame))
        {
            if (!stop_requested.load())
            {
                std::cerr << "Failed to read frame\n";
                capture_failed.store(true);
            }

            stop_requested.store(true);
            frame_buffer.stop();

            return;
        }

        frame_buffer.push(std::move(frame));
    }
}

void render_loop(std::shared_ptr<renderer::Renderer> renderer, Latest_frame_buffer &frame_buffer,
                 std::atomic<bool> &stop_requested)
{
    std::size_t frame_counter{0};
    double measured_fps{0.0};

    auto fps_start{std::chrono::steady_clock::now()};

    while (!stop_requested.load())
    {
        Frame frame;

        if (!frame_buffer.wait_and_pop(frame))
        {
            break;
        }

        const auto now{std::chrono::steady_clock::now()};

        const double frame_age_ms{std::chrono::duration<double, std::milli>(now - frame.timestamp).count()};

        ++frame_counter;

        const double elapsed{std::chrono::duration<double>(now - fps_start).count()};

        if (elapsed >= fps_measurement_interval_sec)
        {
            measured_fps = static_cast<double>(frame_counter) / elapsed;

            frame_counter = 0;
            fps_start = now;
        }

        const renderer::Render_statistics statistics{measured_fps, frame_age_ms, frame_buffer.dropped_frames()};

        if (!renderer->render(frame, statistics))
        {
            stop_requested.store(true);
            frame_buffer.stop();

            break;
        }
    }

    renderer->close();
}

} // namespace

namespace application
{

Application::Application(Commandline_arguments::Cmd_args args,
                         std::shared_ptr<camera_source::Camera_source> camera_source,
                         std::shared_ptr<renderer::Renderer> renderer,
                         std::unique_ptr<device_validator::Device_validator> device_validator)
    : args_{std::move(args)}, camera_source_{std::move(camera_source)}, renderer_{std::move(renderer)},
      device_validator_{std::move(device_validator)}
{
}

bool Application::run()
{
    if (!device_validator_->is_valid(args_.camera_device, args_.width, args_.height, args_.fps))
    {
        std::cerr << "Requested camera configuration is not supported\n";
        return false;
    }

    if (!camera_source_->open(args_.camera_device, args_.width, args_.height, args_.fps))
    {
        return false;
    }

    Latest_frame_buffer frame_buffer;

    std::atomic<bool> stop_requested{false};
    std::atomic<bool> capture_failed{false};

    std::thread capture_thread{[&] { capture_loop(camera_source_, frame_buffer, stop_requested, capture_failed); }};

    std::thread render_thread{[&] { render_loop(renderer_, frame_buffer, stop_requested); }};

    capture_thread.join();
    render_thread.join();

    return !capture_failed.load();
}

} // namespace application
