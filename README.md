# C++ Camera Preview
C++ application for opening a camera and displaying video preview.

## Requirements
- Linux
- C++17
- CMake
- OpenCV (temporary dependency, may be replaced later)

## Installation needed packages

On Ubuntu/Debian:

```bash
sudo apt update
sudo apt install g++ gcc build-essential cmake libopencv-dev
```

## How to use

Build application

```bash
git clone https://github.com/OShatylo/test_camera_preview.git
cd test_camera_preview
mkdir build
cmake .. && make -j
```

Run application

```bash
./camera_preview
```


