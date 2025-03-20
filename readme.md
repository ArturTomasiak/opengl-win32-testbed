## testbed

as of current, my code only renders a square

### src structure
`includes.h` -> includes, debugging and errors, the latter two of which are implemented in `/error/error.c`
`main.h` -> WinMain, window handling, main loop and everything that is yet to be abstracted
`/vertex` -> vertex and index buffer handling
`/shaders` -> everything related to reading, compiling and linking shaders

### compilation
the setup my bat file was made for consists of installing [UCRT runtime](https://winlibs.com/) and [glew precompiled binaries](https://glew.sourceforge.net/) to C:\, then adding mingw's bin to path