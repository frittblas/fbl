fbl - fritt blas library AKA Hans Advance

It's a cross platform 2d game engine written in C!

It's built on top of SDL2 and makes use of the image, ttf and mixer addons.
Physics are achieved via Chipmunk,
and scripting is done with Lua.

Platforms supported: Windows, MacOSX, HTML5 (emscripten), Raspberry pi.
Planned platforms: Android(on the way), IOS (maybe not).

Started March 6th 2017.

Features (current ver. 0.9.6):

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
- Lua integration
- Multithreading

- Program in C/C++ or Lua or a mix.
- fast, lightweight engine and simple API.


upcoming features (ver 1.0)

- 2d lights
- Networking
- Scene editor
- OpengGL renderer

/Hans

New in version 0.9.5:

Updated dependencies (SDL 2.0.12, SDL image 2.05, SDL mixer 2.04, SDL ttf 2.0.15, chipmunk 7.03, Lua 5.4.1)
Visual studio 2019 project (x86 debug and release, x64 release)
Support for Emscripten 2.07

Just unrar dependencies.rar, project_vs2015.rar and project_vs2019.rar where they are (Extract Here).

Added a particle system.
Lots of tweaks, bugfixes, optimisations etc.
Added a direct reference optimisation system for the sprites and primitives.
Culling system for the sprites and primitives.
Made the demos nicer.
SDL mixer now works in Emscripten without hacks.

New in version 0.9.6:

Added a threadpool. If active, it will delegate some parts of the engine to available threads
in the pool such as particle logic, animation, physics translation, ui etc. this speeds up things
considerably. Also added fbl_submit_task_tp() that executes any function you want on a available thread.
Also did some bugfixes.

Notes:

~150 (146) functions you can call (from C, C++ or Lua) to make 2d game programming
alot easier and kinda fun...

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

A note on intput:
The fbl_get_key_down() and fbl_get_mouse_click() keep returning true as long as the button is held.
The fbl_get_key_up() and fbl_get_mouse_release() keep returning true until any another button is pressed.

If you deactivate a sprite with physics, the physics will remain off even
if you turn it on again. You have to manually turn on physics again, including
any special shape or weight etc. The body needs to be removed entirely.
(Just remove it from the space? And the add again?) Fix until 1.0.

Use .png and .ogg and .ttf files for media.

Includes dependencies and projects for the supported platforms.

The visual studio 2015 and 2019 versions works out of the box (vs2019 has x86 (release and debug) and x64-bit(release) versions),
the Xcode version hasn't been updated for a long time.
RPI version works but you need to build all dependencies, which is not too hard. Start here:
http://www.frittblas.se/misc/rpi-SDL2.html

For the emscripten build, install the latest emscripten version. Place the fbl folder in emsdk-master\upstream\emscripten\
then copy build-masta.bat in fbl\project_emscripten\ to emsdk-master\upstream\emscripten\
then run emcmdprompt.bat and cd to emsdk-master\upstream\emscripten\
then type build-masta

The emscripten output is then located in emsdk-master\upstream\emscripten\fbl\project_emscripten\output

A demo of the functionality: (cycle through demos with F1-F2): http://www.frittblas.se/fbl/output_2/fbl_test.html
Try clicking on stuff and use WASD and arrow keys to control the sprite and the camera in demo 1 :)

to be continued..

