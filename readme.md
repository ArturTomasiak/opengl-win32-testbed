## testbed

as of current, my code renders a textured square that can move with wsad

![gif](https://github.com/user-attachments/assets/241dd16e-05aa-4c1c-b0f6-2e315991190d)

### src structure
`includes` -> includes, debugging and errors, the latter two of which are implemented in `/error/error.c`

`window` -> a header and c file for WinMain, window handling and main loop

`/renderer` -> a header and c file for drawing to screen

`/shaders` -> a header and c file for everything related to reading, compiling and linking shaders

`/texture` -> changing png files to usable textures

`/vertex` -> vertex buffer, index buffer object and vertex array object handling, each in their subsequent c and h files

`/math` -> functions for handling with matricies

### compilation
my set up consists of:
1. install [UCRT runtime](https://winlibs.com/) and [glew precompiled binaries](https://glew.sourceforge.net/) to C:\
2. add mingw's bin to path
3. install [LibPng](https://download.sourceforge.net/libpng/libpng-1.6.42.tar.xz) and [Zlib](https://zlib.net/) sources
4. build zlib via
```
make -f win32/Makefile.gcc
```
5. build LipPng via
```
cmake C:\path_to_libpng_source ^
  -G "MinGW Makefiles" ^
  -DZLIB_LIBRARY=C:/path_to_libz/libz.a ^
  -DZLIB_INCLUDE_DIR=C:/path_to_libz ^
  -DCMAKE_INSTALL_PREFIX=C:/path_to_where_you_want_libpng_installed ^
  -DPNG_SHARED=ON ^
  -DPNG_STATIC=ON

mingw32-make
mingw32-make install
```
6. ensure build.bat paths are correct for your setup

`build.bat` to compile

although one can use msys or the visual studio complete packages instead

delete `#define demidebug` from includes and add `-mwindows` flag to build.bat for release mode
