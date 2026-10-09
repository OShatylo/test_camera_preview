#include "application/application.hpp"
#include "mocks/mock_camera_source.hpp"
#include "mocks/mock_device_validator.hpp"
#include "mocks/mock_renderer.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <memory>

namespace
{

class Application_test : public testing::Test
{
  protected:
    void SetUp() override
    {
        camera_source_ = std::make_shared<Mock_camera_source>();
        renderer_ = std::make_shared<Mock_renderer>();

        auto device_validator = std::make_unique<Mock_device_validator>();
        device_validator_ = device_validator.get();

        application_ = std::make_unique<application::Application>(
            args_,
            camera_source_,
            renderer_,
            std::move(device_validator));
    }

    Commandline_arguments::Cmd_args args_{
        "/dev/video0",
        1280,
        720,
        30};

    std::shared_ptr<Mock_camera_source> camera_source_;
    std::shared_ptr<Mock_renderer> renderer_;

    Mock_device_validator *device_validator_{nullptr};

    std::unique_ptr<application::Application> application_;
};

TEST_F(Application_test, DoesNotOpenCameraWhenConfigurationIsInvalid)
{
    EXPECT_CALL(*device_validator_,
                is_valid(args_.camera_device, args_.width, args_.height, args_.fps))
        .WillOnce(testing::Return(false));

    EXPECT_CALL(*camera_source_, open(testing::_, testing::_, testing::_, testing::_)).Times(0);
    EXPECT_CALL(*camera_source_, read(testing::_)).Times(0);
    EXPECT_CALL(*renderer_, render(testing::_, testing::_)).Times(0);
    EXPECT_CALL(*renderer_, close()).Times(0);

    EXPECT_FALSE(application_->run());
}

TEST_F(Application_test, ReturnsFalseWhenCameraCannotBeOpened)
{
    EXPECT_CALL(*device_validator_,
                is_valid(args_.camera_device, args_.width, args_.height, args_.fps))
        .WillOnce(testing::Return(true));

    EXPECT_CALL(*camera_source_,
                open(args_.camera_device, args_.width, args_.height, args_.fps))
        .WillOnce(testing::Return(false));

    EXPECT_CALL(*camera_source_, read(testing::_)).Times(0);
    EXPECT_CALL(*renderer_, render(testing::_, testing::_)).Times(0);
    EXPECT_CALL(*renderer_, close()).Times(0);

    EXPECT_FALSE(application_->run());
}

TEST_F(Application_test, StopsSuccessfullyWhenRendererRequestsStop)
{
    EXPECT_CALL(*device_validator_,
                is_valid(args_.camera_device, args_.width, args_.height, args_.fps))
        .WillOnce(testing::Return(true));

    EXPECT_CALL(*camera_source_,
                open(args_.camera_device, args_.width, args_.height, args_.fps))
        .WillOnce(testing::Return(true));

    EXPECT_CALL(*camera_source_, read(testing::_))
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Invoke([](Frame &frame) {
            frame.timestamp = std::chrono::steady_clock::now();
            return true;
        }));

    EXPECT_CALL(*renderer_, render(testing::_, testing::_))
        .WillOnce(testing::Return(false));

    EXPECT_CALL(*renderer_, close()).Times(1);

    EXPECT_TRUE(application_->run());
}

} // namespace
