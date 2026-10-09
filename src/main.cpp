#include "application/application.hpp"
#include "camera/camera_source_opencv.hpp"
#include "camera/device_validator_linux.hpp"
#include "cli/commandline_arguments_parser.hpp"
#include "renderer/renderer_opencv.hpp"

#include <cstdlib>
#include <memory>

int main(int argc, char *argv[])
{
    Commandline_arguments::Commandline_arguments_parser arguments_parser{argc, argv};

    if (!arguments_parser.is_valid())
    {
        return EXIT_FAILURE;
    }

    auto camera_source{std::make_shared<camera_source::Camera_source_opencv>()};

    auto renderer{std::make_shared<renderer::Renderer_opencv>()};

    auto device_validator{std::make_unique<device_validator::Device_validator_linux>()};

    application::Application app{arguments_parser.get_args(), std::move(camera_source), std::move(renderer),
                                 std::move(device_validator)};

    return app.run() ? EXIT_SUCCESS : EXIT_FAILURE;
}
