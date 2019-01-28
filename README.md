## Building
Make sure to build with clang for C++14 support. 
(Ensure default c++ compiler with `update-alternatives --config c++`)

## Building in VSCode
Use the meson-cmake-wrapper
https://github.com/prozum/meson-cmake-wrapper
1. Set `cmake.cmakePath` to `mcw` in workspace settings
2. Create empty CMakeLists.txt in the root directory
3. Shift+F5 works as expected

### Building for Pocket C.H.I.P.
Build with a qemu chroot using Debian Jessie. See "crosscompiling_notes.txt". 

## Submodules
bubbles

## Requirements
libsdl2-dev libsdl-pango-dev libsdl2-image-dev 

## Temperature sensor
Having some good luck so far with this library:
https://github.com/hbt/TEMPered

