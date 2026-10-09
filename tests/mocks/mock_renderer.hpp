#ifndef CAMERA_PREVIEW_TESTS_MOCKS_MOCK_RENDERER_HPP
#define CAMERA_PREVIEW_TESTS_MOCKS_MOCK_RENDERER_HPP

#include "interfaces/renderer.hpp"

#include <gmock/gmock.h>

class Mock_renderer : public renderer::Renderer
{
  public:
    MOCK_METHOD(bool, render, (Frame & frame, const renderer::Render_statistics &statistics), (override));
    MOCK_METHOD(void, close, (), (override));
};

#endif // CAMERA_PREVIEW_TESTS_MOCKS_MOCK_RENDERER_HPP
