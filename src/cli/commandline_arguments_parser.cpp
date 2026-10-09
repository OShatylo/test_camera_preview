#include "cli/commandline_arguments_parser.hpp"

#include <charconv>
#include <getopt.h>
#include <iostream>
#include <string_view>

namespace
{
enum class cmd_parameters : std::uint32_t
{
    device = 1618,
    width,
    height,
    fps,
    help
};

} // namespace

namespace Commandline_arguments
{

Commandline_arguments_parser::Commandline_arguments_parser(int argc, char *argv[]) : is_valid_{parse(argc, argv)}
{
}

void Commandline_arguments_parser::print_usage(const char *application_name)
{
    std::cout << "Usage " << application_name << " --device 0 --width 800 --height 600 --fps 30 \n"
              << "Parameters description: \n"
              << "--device  camera device\n"
              << "--width   frame width\n"
              << "--height  frame height\n"
              << "--fps     frame per second\n"
              << "--help    help \n ";
}

bool Commandline_arguments_parser::is_valid() const
{
    return is_valid_;
}

const Cmd_args &Commandline_arguments_parser::get_args() const
{
    return args_;
}

bool Commandline_arguments_parser::parse(int argc, char *argv[])
{
    const option long_options[] = {{"device", required_argument, nullptr, static_cast<int>(cmd_parameters::device)},
                                   {"width", required_argument, nullptr, static_cast<int>(cmd_parameters::width)},
                                   {"height", required_argument, nullptr, static_cast<int>(cmd_parameters::height)},
                                   {"fps", required_argument, nullptr, static_cast<int>(cmd_parameters::fps)},
                                   {"help", no_argument, nullptr, static_cast<int>(cmd_parameters::help)},
                                   {nullptr, 0, nullptr, 0}};

    while (true)
    {
        const auto option{getopt_long(argc, argv, "", long_options, nullptr)};

        if (option == -1)
        {
            break;
        }

        switch (option)
        {
        case static_cast<int>(cmd_parameters::device):
            if (!parse_device(optarg, args_.camera_device))
            {
                std::cerr << "Invalid device\n";
                return false;
            }
            break;

        case static_cast<int>(cmd_parameters::width):
            if (!parse_positive_uint(optarg, args_.width))
            {
                std::cerr << "Invalid width: " << optarg << "\n";
                return false;
            }
            break;

        case static_cast<int>(cmd_parameters::height):
            if (!parse_positive_uint(optarg, args_.height))
            {
                std::cerr << "Invalid height: " << optarg << "\n";
                return false;
            }
            break;

        case static_cast<int>(cmd_parameters::fps):
            if (!parse_positive_uint(optarg, args_.fps))
            {
                std::cerr << "Invalid FPS: " << optarg << "\n";
                return false;
            }
            break;

        case static_cast<int>(cmd_parameters::help):
            print_usage(argv[0]);
            std::exit(EXIT_SUCCESS);

        default:
            return false;
        }
    }

    if (optind < argc)
    {
        std::cerr << "Unexpected argument: " << argv[optind] << "\n";

        return false;
    }

    return true;
}

bool Commandline_arguments_parser::parse_positive_uint(const char *value, std::uint32_t &result)
{
    if (value == nullptr)
    {
        return false;
    }

    const std::string_view input{value};

    if (input.empty())
    {
        return false;
    }

    std::uint32_t parsed_value{0};

    const auto [ptr, error]{std::from_chars(input.data(), input.data() + input.size(), parsed_value)};

    if ((error != std::errc{}) || (ptr != input.data() + input.size()) || (parsed_value == 0))
    {
        return false;
    }

    result = parsed_value;

    return true;
}

bool Commandline_arguments_parser::parse_device(const char *value, std::string &device)
{
    if (value == nullptr)
    {
        return false;
    }

    const std::string_view input{value};

    if (input.empty())
    {
        return false;
    }

    device = value;

    return true;
}

} // namespace Commandline_arguments
