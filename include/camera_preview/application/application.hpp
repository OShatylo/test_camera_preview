#ifndef CAMERA_PREVIEW_APPLICATION_APPLICATION_HPP
#define CAMERA_PREVIEW_APPLICATION_APPLICATION_HPP

#include "cli/commandline_arguments_parser.hpp"
#include "interfaces/device_validator.hpp"

namespace application
{

class Application
{
  public:
    Application(Commandline_arguments::Cmd_args args, device_validator::Device_validator &device_validator);

    bool run();

  private:
    Commandline_arguments::Cmd_args args_;
    device_validator::Device_validator &device_validator_;
};

} // namespace application

#endif // CAMERA_PREVIEW_APPLICATION_APPLICATION_HPP
