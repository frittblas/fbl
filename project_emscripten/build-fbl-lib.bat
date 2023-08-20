emcc fbl/src/fbl.c ^
fbl/src/sprite.c ^
fbl/src/pathfind.c ^
fbl/src/primitives.c ^
fbl/src/text.c ^
fbl/src/sound.c ^
fbl/src/input.c ^
fbl/src/lua.c ^
fbl/src/physics.c ^
fbl/src/ui.c ^
fbl/src/particles.c ^
fbl/src/thread.c ^
fbl/src/network.c ^
fbl/src/a_star/a_star.c ^
fbl/src/dl_list/dllist.c ^
-O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 ^
-s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 ^
-r -o fbl/dependencies/emscripten/fbl.bc
