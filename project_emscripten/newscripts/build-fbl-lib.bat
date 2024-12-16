emcc -c fbl/src/fbl.c -o fbl/obj/fbl.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/sprite.c -o fbl/obj/sprite.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/pathfind.c -o fbl/obj/pathfind.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/primitives.c -o fbl/obj/primitives.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/text.c -o fbl/obj/text.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/sound.c -o fbl/obj/sound.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/input.c -o fbl/obj/input.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/lua.c -o fbl/obj/lua.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/physics.c -o fbl/obj/physics.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/ui.c -o fbl/obj/ui.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/particles.c -o fbl/obj/particles.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/thread.c -o fbl/obj/thread.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/network.c -o fbl/obj/network.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/a_star/a_star.c -o fbl/obj/a_star.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2
emcc -c fbl/src/dl_list/dllist.c -o fbl/obj/dllist.o -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2

emar rc fbl/dependencies/emscripten/fbl.a fbl/obj/*.o