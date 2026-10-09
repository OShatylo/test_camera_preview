#ifndef CAMERA_PREVIEW_CLI_COMMANDLINE_ARGUMENTS_PARSER_HPP
#define CAMERA_PREVIEW_CLI_COMMANDLINE_ARGUMENTS_PARSER_HPP

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

namespace Commandline_arguments
{

struct Cmd_args
{
    std::string camera_device{"/dev/video0"};
    std::uint32_t width{0};
    std::uint32_t height{0};
    std::uint32_t fps{0};
};

class Commandline_arguments_parser
{
  public:
    Commandline_arguments_parser(int argc, char *argv[]);
    bool is_valid() const;

    const Cmd_args &get_args() const;
    static void print_usage(const char *application_name);

  private:
    bool parse(int argc, char *argv[]);

    static bool parse_positive_uint(const char *value, std::uint32_t &result);
    static bool parse_device(const char *value, std::string &device);

    Cmd_args args_{};
    bool is_valid_{false};
    bool is_help_requested_{false};
};

} // namespace Commandline_arguments

#endif // CAMERA_PREVIEW_CLI_COMMANDLINE_ARGUMENTS_PARSER_HPP
