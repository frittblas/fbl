em++ fbl/game_projects/Ca2/Addons.cpp ^
fbl/game_projects/Ca2/Ca2.cpp ^
fbl/game_projects/Ca2/Chars.cpp ^
fbl/game_projects/Ca2/Efx.cpp ^
fbl/game_projects/Ca2/Game.cpp ^
fbl/game_projects/Ca2/Location.cpp ^
fbl/game_projects/Ca2/LuaDialogue.cpp ^
fbl/game_projects/Ca2/Objects.cpp ^
fbl/game_projects/Ca2/Progress.cpp ^
fbl/game_projects/Ca2/Robots.cpp ^
fbl/game_projects/Ca2/SoundManager.cpp ^
fbl/game_projects/Ca2/SysManager.cpp ^
fbl/game_projects/Ca2/UserInput.cpp ^
fbl/game_projects/Ca2/Weather.cpp ^
fbl/game_projects/Ca2/GameState/Dialogue.cpp ^
fbl/game_projects/Ca2/GameState/Explore.cpp ^
fbl/game_projects/Ca2/GameState/GameState.cpp ^
fbl/game_projects/Ca2/GameState/Maintenance.cpp ^
fbl/game_projects/Ca2/GameState/RobotCollection.cpp ^
fbl/game_projects/Ca2/GameState/Settings.cpp ^
fbl/game_projects/Ca2/GameState/Title.cpp ^
fbl/game_projects/Ca2/GameState/Dungeon/Dungeon.cpp ^
fbl/game_projects/Ca2/GameState/Race/Race.cpp ^
fbl/game_projects/Ca2/GameState/Race/Maze.cpp ^
fbl/game_projects/Ca2/GameState/Race/PostRace.cpp ^
fbl/game_projects/Ca2/GameState/Race/GameModes/CaptureFlags.cpp ^
fbl/game_projects/Ca2/GameState/Race/GameModes/DeathMatch.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/CameraSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/DialogueTrigSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/LightSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/MouseCtrlSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/PathSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/SpriteSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/Race/AutoAimSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/Race/BasicAISystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/Race/DiagSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/Race/HealSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/Race/LaserSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/Race/MagnetSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/Race/PathLogicSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/Race/RobotCtrlSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/Race/ShieldSystem.cpp ^
fbl/game_projects/Ca2/Ecs/Systems/Race/TurboSystem.cpp ^
fbl/tools/ScenEdit/ScenEdit.cpp ^
fbl/tools/scenEdit/Disk.cpp ^
fbl/tools/scenEdit/GuiFuncs.cpp ^
-O2 -s TOTAL_MEMORY=67108864 -s USE_SDL=2 -s USE_SDL_IMAGE=2 ^
-s SDL2_IMAGE_FORMATS="[""png""]" -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2 ^
--preload-file fbl/project_emscripten/output/assets ^
-o fbl/project_emscripten/output/fbl_test.html fbl/dependencies/emscripten/lua/liblua.a ^
fbl/dependencies/emscripten/chipmunk-7.0.3/chipmunk.a ^
fbl/dependencies/emscripten/fbl.a
