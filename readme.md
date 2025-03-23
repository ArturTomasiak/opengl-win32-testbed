## testbed

as of current, my code only renders a square

### src structure
`includes` -> includes, debugging and errors, the latter two of which are implemented in `/error/error.c`

`window` -> a header and c file for WinMain, window handling, main loop and everything that is yet to be abstracted

`/vertex` -> vertex buffer, index buffer object and vertex array object handling, each in their subsequent c and h files

`/shaders` -> a header and c file for everything related to reading, compiling and linking shaders

### compilation
My set up consists of:
1. install [UCRT runtime](https://winlibs.com/) and [glew precompiled binaries](https://glew.sourceforge.net/) to C:\
2. add mingw's bin to path

`build.bat` to compile