#include "core/latest_frame_buffer.hpp"

#include <utility>

void Latest_frame_buffer::push(Frame frame)
{
    {
        std::lock_guard<std::mutex> lock{mutex_};

        if (stopped_)
        {
            return;
        }

        if (has_frame_)
        {
            ++dropped_frames_;
        }

        latest_frame_ = std::move(frame);
        has_frame_ = true;
    }

    condition_variable_.notify_one();
}

bool Latest_frame_buffer::wait_and_pop(Frame &frame)
{
    std::unique_lock<std::mutex> lock{mutex_};

    condition_variable_.wait(lock, [this] { return has_frame_ || stopped_; });

    if (stopped_)
    {
        return false;
    }

    frame = std::move(latest_frame_);
    has_frame_ = false;

    return true;
}

void Latest_frame_buffer::stop()
{
    {
        std::lock_guard<std::mutex> lock{mutex_};

        stopped_ = true;
    }

    condition_variable_.notify_all();
}

std::uint64_t Latest_frame_buffer::dropped_frames() const
{
    std::lock_guard<std::mutex> lock{mutex_};

    return dropped_frames_;
}
