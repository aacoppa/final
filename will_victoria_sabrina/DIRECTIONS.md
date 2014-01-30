# Note to DW:
Sorry, Mr. Dyrland-Weaver, we ran into a snag with the networking
portion of the game, so the project doesn't work as of yet. The UI and
game logic are done, awaiting function calls from the networking
portion. If you make and run ./RISK you'll see our beautiful UI (which
prints info in the terminal based on interactions--better user
feedback requires info to be passed from the network). Hopefully we'll
have it for you tomorrow (Jan 30). So sorry about that.
-- The RISK Team

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
