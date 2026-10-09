#ifndef CAMERA_PREVIEW_TESTS_MOCKS_MOCK_CAMERA_SOURCE_HPP
#define CAMERA_PREVIEW_TESTS_MOCKS_MOCK_CAMERA_SOURCE_HPP

#include "interfaces/camera_source.hpp"

#include <gmock/gmock.h>

class Mock_camera_source : public camera_source::Camera_source
{
  public:
    MOCK_METHOD(bool,
                open,
                (const std::string &camera_device, std::uint32_t width, std::uint32_t height, std::uint32_t fps),
                (override));

    MOCK_METHOD(bool, read, (Frame & frame), (override));
};

#endif // CAMERA_PREVIEW_TESTS_MOCKS_MOCK_CAMERA_SOURCE_HPP
