diff --git a/game_projects/Ca2/Chars.cpp b/game_projects/Ca2/Chars.cpp
index 6dce3cd..0231910 100644
--- a/game_projects/Ca2/Chars.cpp
+++ b/game_projects/Ca2/Chars.cpp
@@ -98,7 +98,7 @@ void Chars::setupNpc(Game& g) {
 
 					switch (g.mMap->tile[index]->type) {
 
-						case 10:	// Npc with type 10 (-10) = 0 = slime
+						case 10:		// Npc with type 10 (-10) = 0 = slime
 						case 11:	// type 11 (-10) = 1 = also slime :)
 						case 30:
 						case 50:
@@ -110,7 +110,7 @@ void Chars::setupNpc(Game& g) {
 							uint8_t dialogueId = g.mMap->tile[index]->type - NpcOffset;
 							g.mEcs->AddComponent(id, Position{ x, y });
 							g.mEcs->AddComponent(id, DialogueTrigger{ dialogueId });
-														// id  tx  ty   w    h  scale
+														  // id  tx  ty   w    h  scale
 							//g.mEcs->AddComponent(id, Light{ 0, 384, 0, 128, 128, 2.0 });
 							std::cout << "Npc added." << std::endl;
 							break;
diff --git a/game_projects/Ca2/Chars.hpp b/game_projects/Ca2/Chars.hpp
index 4e0f084..0d19ccb 100644
--- a/game_projects/Ca2/Chars.hpp
+++ b/game_projects/Ca2/Chars.hpp
@@ -22,13 +22,10 @@ class Chars {
 
 public:
 
-    // Could use normal enum here, so we can use the enum as array index directly.
-    // Now we have to use static_cast, see below
     enum NpcName {
         StorySlime,
         InfoSlime,
-        EventSlime,
-        NumChars
+        EventSlime
     };
 
     Entity mBrodo;  // the player!
diff --git a/game_projects/Ca2/GameState/GameState.cpp b/game_projects/Ca2/GameState/GameState.cpp
index 736d185..5628dfe 100644
--- a/game_projects/Ca2/GameState/GameState.cpp
+++ b/game_projects/Ca2/GameState/GameState.cpp
@@ -46,6 +46,7 @@
 #include "Maintenance.hpp"
 
 #include "../Chars.hpp"
+#include "../Objects.hpp"
 #include "../Efx.hpp"
 #include "../Robots.hpp"
 #include "../Addons.hpp"
@@ -242,6 +243,7 @@ void GameState::toTitle(Game& g) {
 	g.mLocation->unLoadLocation(g.mMap);
 	g.mChars->removePlayer(g.mEcs);	 // delete the player completely
 	g.mChars->removeNpc(g.mEcs);	 // also delete all npcs in the current scene
+	//g.mObjects->removeObjects(g.mEcs);	 // also delete all chests etc.
 	g.mRobots->removeRobots(g.mEcs); // delete all the robots
 	g.mAddons->removeAddons(g.mEcs); // delete all addons
 	fbl_set_clear_color(11, 168, 230, 255);	// blue sky for the title
@@ -261,6 +263,8 @@ void GameState::titleToExplore(Game& g) {
 	g.mChars->setupPlayer(g.mEcs);	// create the player entity and add the right components
 	g.mChars->setupNpc(g);			// add all npcs based on the map file
 
+	//g.mObjects->setupObjects(g);		// chests etc.
+
 	g.mProgress->resetProgress();	// reset funds and completed races etc.
 
 	g.mRobots->setupRobots(g.mEcs); // create the robot entities and add the basic components
diff --git a/game_projects/Ca2/LuaDialogue.cpp b/game_projects/Ca2/LuaDialogue.cpp
index 7f2769e..52b0274 100644
--- a/game_projects/Ca2/LuaDialogue.cpp
+++ b/game_projects/Ca2/LuaDialogue.cpp
@@ -14,6 +14,8 @@
 #include "../../src/fbl.hpp"
 #include "../../dependencies/common/lua-5.4.1/include/lua.hpp"
 #include "Game.hpp"
+#include "Progress.hpp"
+
 #include "GameState/GameState.hpp"
 #include "LuaDialogue.hpp"
 
@@ -43,6 +45,8 @@ int luaDisplayDialog(lua_State* lua_env);
 int luaGetResponse(lua_State* lua_env);
 int luaGetCurrentDialogueId(lua_State* lua_env);
 
+//int luaGiveFunds(lua_State* lua_env);
+
 // register these C++ functions so they can be called from Lua.
 void registerFuncsToLua()
 {
@@ -56,6 +60,8 @@ void registerFuncsToLua()
 
 	lua_register(fbl_lua_env, "getCurrentDialogueId", luaGetCurrentDialogueId);
 
+	//lua_register(fbl_lua_env, "giveFunds", luaGiveFunds);
+
 }
 
 void initLuaDialog() {
@@ -112,7 +118,7 @@ void initLuaDialog() {
 	fbl_set_ui_elem_access(fDiaButtonNo, 60);
 
 	// the "Talk" text and button
-	fDiaTextTalk = fbl_create_text(255, 255, 255, 0, (char*)"Talk");
+	fDiaTextTalk = fbl_create_text(255, 255, 255, 0, (char*)"Interact");
 	fbl_set_text_align(fDiaTextTalk, FBL_ALIGN_LEFT);
 	fbl_set_text_xy(fDiaTextTalk, x, 32);
 
@@ -259,3 +265,16 @@ int luaGetCurrentDialogueId(lua_State* lua_env) {
 	return 1;
 
 }
+/*
+int luaGiveFunds(lua_State* lua_env) {
+
+	int amount = rand() % 5 + 3;
+
+	gGame->mProgress->mFunds += amount;
+
+	lua_pushnumber(lua_env, amount);
+
+	return 1;
+
+}
+*/
\ No newline at end of file
diff --git a/game_projects/Ca2/Objects.cpp b/game_projects/Ca2/Objects.cpp
index 4638d09..ea38c92 100644
--- a/game_projects/Ca2/Objects.cpp
+++ b/game_projects/Ca2/Objects.cpp
@@ -4,13 +4,17 @@
 *
 *	Objects.cpp
 *
-*	Objects class implementation, keeps references to the game objects, movable rocks, shops etc.
+*	Objects class implementation, keeps references to the game objects, chests, secret rocks etc.
 *
 *	Hans Strömquist 2022
 *
 */
 
-#include <iostream>
+#include "../../tools/ScenEdit/ScenEdit.hpp"
+#include "../../tools/ScenEdit/GuiFuncs.hpp"
+#include "Ecs/Ecs.hpp"
+#include "Ecs/Components.hpp"
+#include "Game.hpp"
 #include "Objects.hpp"
 
 // Objects-class implementation
@@ -25,3 +29,51 @@ Objects::~Objects() {
 	std::cout << "Destroyed Objects subsystem." << std::endl;
 
 }
+
+void Objects::setupObjects(Game& g) {
+
+	// Objects start at tile->type == 100
+	// type 0 - 9 are reserved for different terrain
+	// 10 - 99 is npcs
+
+	const int ObjectOffset = 10;
+
+	for (int i = 0; i < Game::MapW; i++) {
+		for (int j = 0; j < Game::MapH; j++) {
+			int index = i + g.mMap->mapWidth * j;
+			if (g.mMap->tile[index] != nullptr)
+				if (g.mMap->tile[index]->type > Chest - 1)	// if the type is 10 or more = not terrain anymore but Npc!
+
+					switch (g.mMap->tile[index]->type) {
+
+						case Chest:	// Object with type 100 (-10) = 0 = chest
+
+							int id = g.mEcs->CreateEntity();
+							g.mObjects->mObject.push_back(id);
+							float x = g.mMap->tile[index]->x;
+							float y = g.mMap->tile[index]->y;
+							uint8_t dialogueId = g.mMap->tile[index]->type - ObjectOffset;
+							g.mEcs->AddComponent(id, Position{ x, y });
+							g.mEcs->AddComponent(id, DialogueTrigger{ dialogueId });
+														  // id  tx  ty   w    h  scale
+							//g.mEcs->AddComponent(id, Light{ 0, 384, 0, 128, 128, 2.0 });
+							std::cout << "Object added." << std::endl;
+							break;
+
+					}
+
+		}
+	}
+
+}
+
+void Objects::removeObjects(Coordinator* mEcs) {
+
+	for (Entity e : mObject) {
+		mEcs->DestroyEntity(e);
+		std::cout << "Object removed." << std::endl;
+	}
+
+	mObject.clear();
+
+}
diff --git a/game_projects/Ca2/Objects.hpp b/game_projects/Ca2/Objects.hpp
index a476de8..2282fc7 100644
--- a/game_projects/Ca2/Objects.hpp
+++ b/game_projects/Ca2/Objects.hpp
@@ -19,9 +19,17 @@
 class Objects {
 
 public:
+
+    enum ObjectName {
+        Chest = 100
+    };
+
     Objects();
     ~Objects();
 
-    std::vector<Entity> mMoveBlock; // list of all the movable blocks in a scene
+    std::vector<Entity> mObject; // list of all the objects in the scene (chests etc.)
+
+    void setupObjects(Game& g);
+    void removeObjects(Coordinator* mEcs);
 
 };
diff --git a/project_vs2019/.vs/fbl_project/v16/.suo b/project_vs2019/.vs/fbl_project/v16/.suo
index 0d94156..aa404ef 100644
Binary files a/project_vs2019/.vs/fbl_project/v16/.suo and b/project_vs2019/.vs/fbl_project/v16/.suo differ
diff --git a/project_vs2019/x64/Release/assets/Ca2Dialogue.lua b/project_vs2019/x64/Release/assets/Ca2Dialogue.lua
index 1e2acf1..d15b804 100644
--- a/project_vs2019/x64/Release/assets/Ca2Dialogue.lua
+++ b/project_vs2019/x64/Release/assets/Ca2Dialogue.lua
@@ -123,6 +123,8 @@ end
 -- If you don't want anything on lines 2 and 3, just leave one empy space (" ")! Nothing else.
 -- If you only want one character as reply 2, type "? ", so it gets picked up correctly.
 
+--- Story Slimes ---
+
 g_dialogue[0] = coroutine.create(function ()
 local iter = 1
 while true do
@@ -211,6 +213,8 @@ while true do
 end
 end)
 
+--- Info Slimes ---
+
 g_dialogue[20] = coroutine.create(function ()
 local iter = 1
 while true do
@@ -241,12 +245,40 @@ while true do
 end
 end)
 
+--- Event Slimes ---
+
 g_dialogue[40] = coroutine.create(function ()
 local iter = 1
 while true do
 	if iter == 1 then
 		if not g_wait_response then
 			disp_dw("I am the event-slime.", "I can arrange a race event.", "Do you want to race?", "Yes", "No")
+		elseif getResponse() == OK then
+			iter = advance(Explore, 2)
+		end
+	elseif iter == 2 then
+		if not g_wait_response then
+			disp_dw("It's empty.", " ", " ", "Ok", " ")
+		elseif getResponse() == OK then
+			iter = advance(Explore, 2) -- go to explore and stay at this option
+		end
+	end
+
+	coroutine.yield()
+	
+end
+end)
+
+--- Objects ---
+
+--- Chest ---
+--[[
+g_dialogue[90] = coroutine.create(function ()
+local iter = 1
+while true do
+	if iter == 1 then
+		if not g_wait_response then
+			disp_dw("You found: " + giveFunds() + " coins!", " ", " ", "Nice", " ")
 		elseif getResponse() == YES then
 			iter = advance(Race, 1)
 		elseif getResponse() == NO then
@@ -259,6 +291,8 @@ while true do
 end
 end)
 
+]]
+
 function fbl_lua_start()
 
 	make_strict()	-- have to declare variables beforehand
diff --git a/project_vs2019/x64/Release/assets/map.scn b/project_vs2019/x64/Release/assets/map.scn
index 2b32533..3bc4145 100644
--- a/project_vs2019/x64/Release/assets/map.scn
+++ b/project_vs2019/x64/Release/assets/map.scn
@@ -244,6 +244,7 @@ ScenEditMap
 1888 96 0 416 0 0 1 0 1 10
 0 128 0 416 0 0 1 0 1 10
 32 128 0 416 0 0 1 0 1 10
+128 128 96 480 0 0 100 0 1 10
 224 128 32 416 0 0 1 0 1 10
 480 128 0 416 0 0 1 0 1 10
 512 128 0 416 0 0 1 0 1 10
diff --git a/project_vs2019/x64/Release/assets/map/01.scn b/project_vs2019/x64/Release/assets/map/01.scn
index 2b32533..3bc4145 100644
--- a/project_vs2019/x64/Release/assets/map/01.scn
+++ b/project_vs2019/x64/Release/assets/map/01.scn
@@ -244,6 +244,7 @@ ScenEditMap
 1888 96 0 416 0 0 1 0 1 10
 0 128 0 416 0 0 1 0 1 10
 32 128 0 416 0 0 1 0 1 10
+128 128 96 480 0 0 100 0 1 10
 224 128 32 416 0 0 1 0 1 10
 480 128 0 416 0 0 1 0 1 10
 512 128 0 416 0 0 1 0 1 10
diff --git a/project_vs2019/x64/Release/assets/sfx/rain_.ogg b/project_vs2019/x64/Release/assets/sfx/rain_.ogg
deleted file mode 100644
index b448da5..0000000
Binary files a/project_vs2019/x64/Release/assets/sfx/rain_.ogg and /dev/null differ
diff --git a/project_vs2019/x64/Release/assets/sfx/summer_.ogg b/project_vs2019/x64/Release/assets/sfx/summer_.ogg
deleted file mode 100644
index c7ed453..0000000
Binary files a/project_vs2019/x64/Release/assets/sfx/summer_.ogg and /dev/null differ
