## testbed

as of current, my code only renders a square

### project structure
/exe -> a folder for dll files and exe file
/resources -> a folder for shaders and textures
/src -> source code
    /src/app.h -> defines everything in my project except for static variables
    /src/main.c -> everything related to window handling and graphics programming
    /src/utilities.c -> file reading and debugging functions

### compilation
the setup my bat file was made for consists of installing [UCRT runtime](https://winlibs.com/) and [glew precompiled binaries](https://glew.sourceforge.net/) to C:\, then adding mingw's bin to path