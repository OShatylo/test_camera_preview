#ifndef CAMERA_PREVIEW_CORE_LATEST_FRAME_BUFFER_HPP
#define CAMERA_PREVIEW_CORE_LATEST_FRAME_BUFFER_HPP

#include "core/frame.hpp"

#include <condition_variable>
#include <cstdint>
#include <mutex>

class Latest_frame_buffer
{
  public:
    void push(Frame frame);

    bool wait_and_pop(Frame &frame);

    void stop();

    std::uint64_t dropped_frames() const;

  private:
    mutable std::mutex mutex_;
    std::condition_variable condition_variable_;

    Frame latest_frame_;

    bool has_frame_{false};
    bool stopped_{false};

    std::uint64_t dropped_frames_{0};
};

#endif // CAMERA_PREVIEW_CORE_LATEST_FRAME_BUFFER_HPP
