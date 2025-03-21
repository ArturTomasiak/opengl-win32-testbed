## testbed

as of current, my code only renders a square

### src structure
`includes` -> includes, debugging and errors, the latter two of which are implemented in `/error/error.c`

`window` -> WinMain, window handling, main loop and everything that is yet to be abstracted

`/vertex` -> vertex buffer, index buffer and TODO vertex_array handling

`/shaders` -> everything related to reading, compiling and linking shaders

### compilation
My set up consists of:
1. install [UCRT runtime](https://winlibs.com/) and [glew precompiled binaries](https://glew.sourceforge.net/) to C:\
2. add mingw's bin to path

`build.bat` to compile