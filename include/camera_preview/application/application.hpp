#ifndef CAMERA_PREVIEW_APPLICATION_APPLICATION_HPP
#define CAMERA_PREVIEW_APPLICATION_APPLICATION_HPP

#include "cli/commandline_arguments_parser.hpp"
#include "interfaces/camera_source.hpp"
#include "interfaces/device_validator.hpp"
#include "interfaces/renderer.hpp"

namespace application
{

class Application
{
  public:
    Application(Commandline_arguments::Cmd_args args, std::shared_ptr<camera_source::Camera_source> camera_source,
                std::shared_ptr<renderer::Renderer> renderer,
                std::unique_ptr<device_validator::Device_validator> device_validator);

    bool run();

  private:
    Commandline_arguments::Cmd_args args_;

    std::shared_ptr<camera_source::Camera_source> camera_source_;
    std::shared_ptr<renderer::Renderer> renderer_;
    std::unique_ptr<device_validator::Device_validator> device_validator_;
};

} // namespace application

#endif // CAMERA_PREVIEW_APPLICATION_APPLICATION_HPP
