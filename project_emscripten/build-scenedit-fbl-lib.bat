em++ fbl/tools/scenEdit/scenEdit.cpp fbl/tools/scenEdit/Disk.cpp -O2 -s TOTAL_MEMORY=67108864 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS="[""png""]" -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2 --preload-file fbl/project_emscripten/output/assets -o fbl/project_emscripten/output/fbl_test.html fbl/dependencies/emscripten/lua/liblua.bc fbl/dependencies/emscripten/chipmunk-7.0.3/chipmunk.bc fbl/dependencies/emscripten/fbl.bc
