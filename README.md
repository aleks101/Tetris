# Requirements
## Git
Git download https://git-scm.com/downloads/win

## Cmake
CMake download https://cmake.org/download/

## Compiler
Make sure to have a compiler installed on your computer https://www.mingw-w64.org/downloads/

# Building/compiling the project
If you have installed and build all the dependencies, you can now build the game. Run cmake script from the build folder and then make to compile the program, after compiling the .exe file will be generated inside build/bin folder

cmake generators https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html

Project root folder:
```
mkdir build
cd build

cmake .. -G "set your generator"
make
```

The library file will be generated inside Build/Source
