# C++ Camera Preview

C++17 Linux application for opening a camera device and displaying a real-time video preview.

The project implements a low-latency two-thread capture/render pipeline. When rendering cannot keep up with capture, older frames may be dropped so that the newest available frame is displayed.

## Dependencies

- Linux
- C++17 compiler
- CMake 3.16+
- OpenCV
- POSIX threads / `std::thread`
- GoogleTest and GoogleMock for unit tests

On Ubuntu/Debian:

```bash
sudo apt update
sudo apt install build-essential cmake libopencv-dev libgtest-dev libgmock-dev
```

## Build

Clone the repository:

```bash
git clone https://github.com/OShatylo/test_camera_preview.git
cd test_camera_preview
```

Configure and build:

```bash
cmake -S . -B build
cmake --build build -j
```

Tests are enabled by default through CTest. To build without tests:

```bash
cmake -S . -B build -DBUILD_TESTING=OFF
cmake --build build -j
```

## Run

Run with the default camera (`/dev/video0`):

```bash
./build/camera_preview_app
```

Use another camera device:

```bash
./build/camera_preview_app --device /dev/video2
```

Request a specific resolution:

```bash
./build/camera_preview_app --device /dev/video0 --width 1280 --height 720
```

Request a specific resolution and FPS:

```bash
./build/camera_preview_app --device /dev/video0 --width 1280 --height 720 --fps 30
```

Show command-line help:

```bash
./build/camera_preview_app --help
```

Close the preview using `Esc`, `q`, or `Q`.

## Tests

Run the test suite with:

```bash
ctest --test-dir build --output-on-failure
```

The current unit tests use GoogleTest and GoogleMock to verify application behavior through mocked camera source, renderer, and device validator dependencies.

Covered scenarios include:

- unsupported camera configuration;
- failure to open the camera;
- successful application shutdown when the renderer requests stop.

## Implemented Levels

### Level 1 — Basic camera preview

Implemented:

- opening the default or explicitly selected Linux camera device;
- live OpenCV preview;
- exit using `Esc`, `q`, or `Q`;
- command-line options:
  - `--device`;
  - `--width`;
  - `--height`;
  - `--fps`;
  - `--help`;
- validation of requested camera parameters using V4L2;
- logging of active resolution, frame format (FOURCC), and camera-reported FPS.

### Level 2 — Real-time / low-latency pipeline

Implemented:

- separate capture and render threads;
- single-slot latest-frame buffer;
- frame dropping when the renderer cannot keep up with capture;
- preference for minimum latency instead of displaying every captured frame;
- frame timestamps and sequence numbers;
- runtime overlay with:
  - measured FPS;
  - frame age in milliseconds;
  - dropped frame count;
- dependency injection for `Camera_source`, `Renderer`, and `Device_validator`;
- unit tests using GoogleTest and GoogleMock.
