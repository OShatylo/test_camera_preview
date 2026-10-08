#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "commandline_arguments_parser.hpp"

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

#endif // APPLICATION_HPP