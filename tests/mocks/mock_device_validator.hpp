#ifndef CAMERA_PREVIEW_TESTS_MOCKS_MOCK_DEVICE_VALIDATOR_HPP
#define CAMERA_PREVIEW_TESTS_MOCKS_MOCK_DEVICE_VALIDATOR_HPP

#include "interfaces/device_validator.hpp"

#include <gmock/gmock.h>

class Mock_device_validator : public device_validator::Device_validator
{
  public:
    MOCK_METHOD(bool,
                is_valid,
                (const std::string &camera_device, std::uint32_t width, std::uint32_t height, std::uint32_t fps),
                (const, override));
};

#endif // CAMERA_PREVIEW_TESTS_MOCKS_MOCK_DEVICE_VALIDATOR_HPP
