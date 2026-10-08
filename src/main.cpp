#include "application.hpp"
#include "commandline_arguments_parser.hpp"

int main(int argc, char *argv[])
{
    Commandline_arguments::Commandline_arguments_parser arguments_parser{argc, argv};

    if (!arguments_parser.is_valid())
    {
        return EXIT_FAILURE;
    }

    application::Application app{arguments_parser.get_args()};

    return app.run() ? EXIT_SUCCESS : EXIT_FAILURE;
}
