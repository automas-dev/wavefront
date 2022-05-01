# Wavefront

A Wavefront OBJ and MTL Parser.

## Building

```sh
git clone https://github.com/twh2898/wavefront.git
cd wavefront
mkdir build
cd build
cmake ..
make
```

### Installing

```sh
cd build
make install
```

## CMake Submodule

```sh
git submodule add https://github.com/twh2898/wavefront.git
git submodule update --init --recursive
```

```cmake
add_subdirectory(Wavefront EXCLUDE_FROM_ALL)
```

## CMake Fetch Content

```cmake
include(FetchContent)
FetchContent_Declare(
    glpp
    GIT_REPOSITORY https://github.com/twh2898/wavefront.git
    GIT_TAG        0.1.0
)
FetchContent_MakeAvailable(glpp)
```

## License

This project uses the [MIT](LICENSE) License.