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
fbl/game_projects/Lotr/Chars.cpp \
fbl/game_projects/Lotr/Location.cpp \
fbl/game_projects/Lotr/Weather.cpp \
fbl/game_projects/Lotr/Game.cpp \
fbl/game_projects/Lotr/Lotr.cpp \
fbl/game_projects/Lotr/LuaDialogue.cpp \
fbl/game_projects/Lotr/Objects.cpp \
fbl/game_projects/Lotr/Progress.cpp \
fbl/game_projects/Lotr/SysManager.cpp \
fbl/game_projects/Lotr/SoundManager.cpp \
fbl/game_projects/Lotr/UserInput.cpp \
fbl/game_projects/Lotr/GameState/Dialogue.cpp \
fbl/game_projects/Lotr/GameState/Explore.cpp \
fbl/game_projects/Lotr/GameState/GameState.cpp \
fbl/game_projects/Lotr/GameState/Settings.cpp \
fbl/game_projects/Lotr/GameState/Title.cpp \
fbl/game_projects/Lotr/Ecs/Systems/CameraSystem.cpp \
fbl/game_projects/Lotr/Ecs/Systems/DialogueTrigSystem.cpp \
fbl/game_projects/Lotr/Ecs/Systems/MouseCtrlSystem.cpp \
fbl/game_projects/Lotr/Ecs/Systems/PathSystem.cpp \
fbl/game_projects/Lotr/Ecs/Systems/SpriteSystem.cpp \
fbl/game_projects/Lotr/Ecs/Systems/LightSystem.cpp \
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
