Release history:

New in version 0.9.8 (2022-04-15):

Added raycasting! New primitive type FBL_RAY.
Added complete support for Android! Read help_res.txt in the project_android folder.
Now works perfectly with C++.
Streamlined the build process for Emscripten and Android.
Made some progress on the Robot-game.
Bug fixes and optimisations.

New in version 0.9.7 (2021-11-11):

Added support for Android, experimental, but works. Read help_res.txt in the project_android folder.
Also some bug fixes and progress on the demo robot-game.

New in version 0.9.6 (2021-03-21):

Added a threadpool. If active, it will delegate some parts of the engine to available threads
in the pool such as particle logic, animation, physics translation, ui etc. this can speed up things
considerably. Also added fbl_submit_task_tp() that executes any function you want on a available thread.
Also did some bugfixes.


New in version 0.9.5 (2020-11-17):

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


Release 0.9 (2019-05-19)

First release of fbl.