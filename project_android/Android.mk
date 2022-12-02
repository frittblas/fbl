LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include

# Add your application source files here...
LOCAL_SRC_FILES := \
fbl/src/fbl.c \
fbl/src/input.c \
fbl/src/lua.c \
fbl/src/particles.c \
fbl/src/pathfind.c \
fbl/src/physics.c \
fbl/src/primitives.c \
fbl/src/sound.c \
fbl/src/sprite.c \
fbl/src/text.c \
fbl/src/thread.c \
fbl/src/network.c \
fbl/src/ui.c \
fbl/game_projects/Ca2/Chars.cpp \
fbl/game_projects/Ca2/Robots.cpp \
fbl/game_projects/Ca2/Location.cpp \
fbl/game_projects/Ca2/Weather.cpp \
fbl/game_projects/Ca2/Game.cpp \
fbl/game_projects/Ca2/Ca2.cpp \
fbl/game_projects/Ca2/LuaDialogue.cpp \
fbl/game_projects/Ca2/Objects.cpp \
fbl/game_projects/Ca2/Progress.cpp \
fbl/game_projects/Ca2/SysManager.cpp \
fbl/game_projects/Ca2/SoundManager.cpp \
fbl/game_projects/Ca2/UserInput.cpp \
fbl/game_projects/Ca2/GameState/RobotCollection.cpp \
fbl/game_projects/Ca2/GameState/Dialogue.cpp \
fbl/game_projects/Ca2/GameState/Explore.cpp \
fbl/game_projects/Ca2/GameState/GameState.cpp \
fbl/game_projects/Ca2/GameState/Settings.cpp \
fbl/game_projects/Ca2/GameState/Title.cpp \
fbl/game_projects/Ca2/GameState/Race/Race.cpp \
fbl/game_projects/Ca2/GameState/Race/Maze.cpp \
fbl/game_projects/Ca2/Ecs/Systems/CameraSystem.cpp \
fbl/game_projects/Ca2/Ecs/Systems/DialogueTrigSystem.cpp \
fbl/game_projects/Ca2/Ecs/Systems/MouseCtrlSystem.cpp \
fbl/game_projects/Ca2/Ecs/Systems/PathSystem.cpp \
fbl/game_projects/Ca2/Ecs/Systems/SpriteSystem.cpp \
fbl/game_projects/Ca2/Ecs/Systems/LightSystem.cpp \
fbl/game_projects/Ca2/Ecs/Systems/Race/LaserSystem.cpp \
fbl/tools/ScenEdit/ScenEdit.cpp \
fbl/tools/scenEdit/Disk.cpp \
fbl/tools/scenEdit/GuiFuncs.cpp \
fbl/src/dl_list/dllist.c \
fbl/src/a_star/a_star.c

LOCAL_SHARED_LIBRARIES := SDL2 \
SDL2_image \
SDL2_mixer \
SDL2_ttf \
SDL2_net \
Chipmunk \
lua

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
