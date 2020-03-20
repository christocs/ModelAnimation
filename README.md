<div align=center>
  <img src="https://i.imgur.com/cRIXVe7.png" alt="ict397-logo" width="200">
  <p>
    ICT397 Assignment
  </p>
</div>

## Table of Contents
* [1&nbsp;&nbsp;Building](#building)
  * [1.1&nbsp;&nbsp;macOS](#macos)
  * [1.2&nbsp;&nbsp;Linux](#linux)
  * [1.3&nbsp;&nbsp;Windows](#windows)
* [2&nbsp;&nbsp;Contributing](#contributing)
* [3&nbsp;&nbsp;Meta](#meta)
  * [3.1&nbsp;&nbsp;License](#license)
  * [3.2&nbsp;&nbsp;Built With](#built-with)

## Building
### macOS
Install build tools:
```
brew install cmake ninja llvm --with-toolchain
```

Clone repository:
```
git clone https://github.com/opeik/ICT397.git
git submodule update --init --recursive --depth 1
```

Generate build files:
```
# Debug
cmake -S . -B build/debug -G Ninja -D CMAKE_BUILD_TYPE=Debug -D CMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/clang++
# Release
cmake -S . -B build/release -G Ninja -D CMAKE_BUILD_TYPE=RelWithDebInfo -D CMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/clang++
```

Compile:
```
cd build/debug && ninja && ./ict397
# Or
cd build/release && ninja && ./ict397
```

### Linux
Install build tools:
```
apt install build-essential clang ninja-build
```

Install dependencies:
```
sudo apt install libgl1-mesa-dev libx11-dev libxrandr-dev libudev-dev \
    libfreetype6-dev libopenal-dev libflac++-dev libvorbis-dev
```

Clone repository:
```
git clone https://github.com/opeik/ICT397.git
git submodule update --init --recursive --depth 1
```

Generate build files:
```
# Debug
cmake -S . -B build/debug -G Ninja -D CMAKE_BUILD_TYPE=Debug -D CMAKE_CXX_COMPILER=clang++
# Release
cmake -S . -B build/release -G Ninja -D CMAKE_BUILD_TYPE=RelWithDebInfo -D CMAKE_CXX_COMPILER=clang++
```

Compile:
```
cd build/debug && ninja && ./ict397
# Or
cd build/release && ninja && ./ict397
```

### Windows
Enable developer mode:
* Open Settings
* Navitgate to Update & Security → For developers
* Enable Developer Mode

Install build tools:
* [Visual Studio Community][2] (2019 or newer)
  * Install the "C++ CMake tools for Windows" component
  * (Optional) Install the "Clang compiler for Windows" component

Clone repository:
```
git clone https://github.com/opeik/ICT397.git
git submodule update --init --recursive --depth 1
```

Compile:
* Open Visual Studio
* Select Open → CMake
* Select `CMakeLists.txt`
* Set the startup item to `ict397.exe`

## Contributing
Please see the [`CONTRIBUTING.md`](CONTRIBUTING.md) file for instructions.

## Meta
### License
This project is licensed under the ISC license. Please see the [`LICENSE.md`](LICENSE.md)
file for details. Individual authors can be found inside the [`AUTHORS.md`](AUTHORS.md) file.

### Built With
TODO

[1]: https://github.com/microsoft/vcpkg#quick-start
[2]: https://visualstudio.microsoft.com/downloads/
