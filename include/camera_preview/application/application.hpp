#ifndef CAMERA_PREVIEW_APPLICATION_APPLICATION_HPP
#define CAMERA_PREVIEW_APPLICATION_APPLICATION_HPP

#include "cli/commandline_arguments_parser.hpp"

namespace application
{

class Application
{
  public:
    explicit Application(Commandline_arguments::Cmd_args args);
    bool run();

  private:
    Commandline_arguments::Cmd_args args_;
};

} // namespace application

#endif // CAMERA_PREVIEW_APPLICATION_APPLICATION_HPP
