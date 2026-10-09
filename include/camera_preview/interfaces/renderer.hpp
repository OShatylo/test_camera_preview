#ifndef CAMERA_PREVIEW_INTERFACES_RENDERER_HPP
#define CAMERA_PREVIEW_INTERFACES_RENDERER_HPP

#include "core/frame.hpp"

#include <cstdint>

namespace renderer
{

struct Render_statistics
{
    double fps{0.0};
    double frame_age_ms{0.0};
    std::uint64_t dropped_frames{0};
};

class Renderer
{
  public:
    virtual ~Renderer() = default;

    virtual bool render(Frame &frame, const Render_statistics &statistics) = 0;

    virtual void close() = 0;
};

} // namespace renderer

#endif // CAMERA_PREVIEW_INTERFACES_RENDERER_HPP
