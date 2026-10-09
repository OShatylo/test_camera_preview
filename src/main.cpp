#include "application/application.hpp"
#include "camera/device_validator_linux.hpp"
#include "cli/commandline_arguments_parser.hpp"

#include <cstdlib>

int main(int argc, char *argv[])
{
    Commandline_arguments::Commandline_arguments_parser arguments_parser{argc, argv};

    if (!arguments_parser.is_valid())
    {
        return EXIT_FAILURE;
    }

    device_validator::Device_validator_linux device_validator;

    application::Application app{arguments_parser.get_args(), device_validator};

    return app.run() ? EXIT_SUCCESS : EXIT_FAILURE;
}
