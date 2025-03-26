## testbed

as of current, my code renders a textured square

### src structure
`includes` -> includes, debugging and errors, the latter two of which are implemented in `/error/error.c`

`window` -> a header and c file for WinMain, window handling and main loop

`/dependencies` -> code not written by me; as of current, it only contains stb_image

`/renderer` -> a header and c file for drawing to screen

`/shaders` -> a header and c file for everything related to reading, compiling and linking shaders

`/texture` -> changing png files to usable textures

`/vertex` -> vertex buffer, index buffer object and vertex array object handling, each in their subsequent c and h files

### compilation
My set up consists of:
1. install [UCRT runtime](https://winlibs.com/) and [glew precompiled binaries](https://glew.sourceforge.net/) to C:\
2. add mingw's bin to path

`build.bat` to compile

delete `#define demidebug` from includes and add `-mwindows` flag to build.bat for release mode