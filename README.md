fbl - fritt blas library AKA Hans Advance

It's a cross platform 2d game engine written in C.

It's built on top of SDL2 and makes use of the image, ttf and mixer addons.
Physics are done with Chipmunk, and scripting with Lua.

Platforms supported: Windows, HTML5 (emscripten), Android(finally done!), Raspberry pi, Ubuntu.
Planned platforms: MacOSX (used to work but don't have a mac anymore), IOS (probably not).

Started March 6th 2017.

Features (current ver. 0.9.9):

- Sprite engine
- Primitives
- Text and fonts
- Physics
- Particle system
- Audio
- UI
- Animation
- Timers, input, camera, debug stuff.
- Pathfinding
- Lua scripting
- Multithreading
- Raycasting
- 2D lights
- Scene editor (tilemaps)

- Program in C/C++ or Lua or a mix.
- Fast, lightweight engine and simple API.


upcoming features (ver 1.0)

- Networking (SDL_net)


/Hans


More than 150 functions you can call (from C, C++ or Lua) to make 2d game programming
easy and insanely fun :|

Sprites, primitives, text objects, particle emitters
and ui objects are stored internally in 5 separate
doubly linked lists.

Game objects are drawn in the order they were created.
Sprites are drawn first.
Primitives are rendered on top of sprites.
Particles on top of primitives.
Text objects on top of particles and
UI is rendered on the very top of everything.

The sprite list can be sorted in layers to make it easier
to create tilemaps w layers and such.

The first item in a list will always be the item with ID = 0 for
all objects except for sprites.
Sprites can be sorted so the first item in the list can be any ID.
Keep FBL_DEBUG on to get error messages about this.

(For now, when deleting the first item in any list it just gets
 deactivated internally, not deleted, to avoid crash. It's fine for now..
Use fbl_destroy_all_sprites() to get rid of everything)

Sprites only get one atlas texture.
Text object only get one font, but you can render one font to a text object
and then close it and open a new one etc..

If main.lua is in the asset folder and fbl_lua_init() is called, the file runs
fbl_lua_start() once and then fbl_lua_loop() every frame and fbl_lua_end() before
the program quits.

Check out main.lua and you will get it.

Random notes:

The fbl_get_key_down() and fbl_get_mouse_click() keep returning true as long as the button is held.
The fbl_get_key_up() and fbl_get_mouse_release() keep returning true until any another button is pressed.

If you deactivate a sprite with physics, the physics will remain off even
if you turn it on again. You have to manually turn on physics again, including
any special shape or weight etc.

Use .png and .ogg and .ttf files for media.

Includes dependencies and projects for Windows (Msvc), webassembly version (Emscripten) and Android (Android Studio, Gradle (see project_android)).

The visual studio 2019 version works out of the box (vs2019 has x86 (release and debug) and x64-bit(release) versions),
the Xcode version has been removed. Don't have a mac atm.
RPI version works but you need to build all dependencies, which is not too hard. Start here:
http://www.frittblas.se/misc/rpi-SDL2.html
Then check project_rpi for detailed insctuctions on how to build fbl for rpi.

Ubuntu is easier (All dependencies can be apt-get installed (no need to build libs yourself))

I will streamline the Linux build-process someday.

For the emscripten build, install the latest emscripten version. Place the fbl folder in emsdk-master\upstream\emscripten\
then copy build-masta.bat in fbl\project_emscripten\ to emsdk-master\upstream\emscripten\
then run emcmdprompt.bat and cd to emsdk-master\upstream\emscripten\
then type build-masta.bat
then run-masta.bat

The emscripten output is then located in emsdk-master\upstream\emscripten\fbl\project_emscripten\output

A demo of the functionality: (cycle through demos with F1-F2): http://www.frittblas.se/fbl/output/fbl_test.html
Try clicking on stuff and use WASD and arrow keys to control the sprite and the camera in demo 1 :)

to be continued..

