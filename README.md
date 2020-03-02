<div align=center>
  <img src="https://i.imgur.com/cRIXVe7.png" alt="ict397-logo" width="200">
  <p>
    ICT397 Assignment
  </p>
</div>

## Table of Contents
* [1&nbsp;&nbsp;Building](#building)
* [2&nbsp;&nbsp;Contributing](#building)
  * [2.1&nbsp;&nbsp;macOS](#macos)
  * [2.2&nbsp;&nbsp;Linux](#linux)
  * [2.3&nbsp;&nbsp;Windows](#windows)
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
git clone --recurse-submodules git@github.com:opeik/ICT397.git
```

Generate build files:
```
cmake . -B build -G Ninja -D CMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/clang++ -D CMAKE_EXPORT_COMPILE_COMMANDS=1
```

Compile:
```
cd build && ninja && ./ict397
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
git clone --recurse-submodules git@github.com:opeik/ICT397.git
```

Generate build files:
```
cmake . -B build -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_EXPORT_COMPILE_COMMANDS=1
```

Compile:
```
cd build && ninja && ./ict397
```

### Windows
Enable developer mode:
* Open Settings
* Navitgate to Update & Security → For developers
* Enable Developer Mode

Install build tools:
* [vcpkg][1]
* [Visual Studio Community][2] (2019 or newer)
  * Install the "C++ CMake tools for Windows" component

Clone repository:
```
git clone --recurse-submodules git@github.com:opeik/ICT397.git
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
