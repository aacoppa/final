# Install

## Ubuntu:
Run the following two lines, which handle downloading, configuring,
compiling, and installing the libraries required.
`curl http://www.libsdl.org/release/SDL2-2.0.0.tar.gz >SDL2.tar.gz &&
tar xvf SDL2.tar.gz && cd SDL2-2.0.0/ && ./configure && make && sudo
make install`  
`curl http://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.12.tar.gz >SDL2_ttf.tar.gz && tar xvf SDL2_ttf.tar.gz && cd SDL2_ttf-2.0.12/ && ./configure && make && sudo make install`


## OSX:
Just use homebrew (so much easier)
`brew install SDL2 && brew install SDL2_ttf`
