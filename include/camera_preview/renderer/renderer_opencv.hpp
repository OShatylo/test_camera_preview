#ifndef CAMERA_PREVIEW_RENDERER_RENDERER_OPENCV_HPP
#define CAMERA_PREVIEW_RENDERER_RENDERER_OPENCV_HPP

#include "interfaces/renderer.hpp"

namespace renderer
{

class Renderer_opencv final : public Renderer
{
  public:
    bool render(Frame &frame, const Render_statistics &statistics) override;

    void close() override;
};

} // namespace renderer

#endif // CAMERA_PREVIEW_RENDERER_RENDERER_OPENCV_HPP
