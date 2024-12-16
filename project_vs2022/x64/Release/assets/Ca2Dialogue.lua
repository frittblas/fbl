--
--
--	Charming Alarming: Reasonable Robots
--
--	Ca2Dialogue.lua
--
--	Contains dialogue stuff for Ca2
--
--	This will be called from your C/C++-program if you call fbl_lua_init()
--	in the beginning of your program. First, the fbl_lua_start(),
--	then, fbl_lua_loop() will be called every frame. Last, fbl_lua_end()
--	will be called when you end the program from C/C++ to clean stuff up.
--
--	You have full access to the fbl api from here!
--  + some special gameplay related Ca2-functions.
--
--	Hans Strömquist 2022
--
--

-- constants

-- game states
Stay = -1
Title = 0
Demo = 1
Settings = 2
Tutorial = 3
Race = 4
Maintenance = 5
--
Explore = 6
Dialogue = 7
RobotCollection = 8

-- number of dialogues
-- NUM_DIALOGUES = 2

-- valid responses

OK	= 1
YES = 1
NO  = 2

-- functions that can be called

-- getState() (returns the above states, except Stay)
-- setState(int state)
-- displayDialog()
-- hideDialog()
-- getResponse() (returns 0 or 1, 2 (OK, YES, NO))
-- getCurrentDialogueId()

-- globals

local g_wait_response = false
local g_dialogue = {}

math.randomseed(os.time())

----------------------------
-- The following function will force you to declare all variables beforehand (great)
----------------------------

function make_strict()
	setmetatable(_G, {
	  __newindex = function (_, n)
		error("attempt to write to undeclared variable "..n, 2)
	  end,
	  __index = function (_, n)
		error("attempt to read undeclared variable "..n, 2)
	  end,
	})
end

-- print some useful debug info

function debug_console(iter)

	print("iter = ", iter)

end

-- set the g_wait_response from C++

function set_lua_not_wait_response()

	g_wait_response = false

end

-- display dialog and set wait for response-flag
function disp_dw(text1, text2, text3, reply1, reply2)

	displayDialog(text1, text2, text3, reply1, reply2)
	g_wait_response = true

end

-- hide dialog and set state
function set_state(state)

	hideDialog()
	setState(state)
	
end

--advance dialogue
function advance(state, iter)

	g_wait_response = false -- not waiting for response anymore
	hideDialog()
	
	if state ~= Stay then	-- only change state if not Staying (in dialogue)
		setState(state)
	end
	
	return iter
	
end

--
-- dialogues for the different characters
--
-- If you don't want anything on lines 2 and 3, just leave one empy space (" ")! Nothing else.
-- If you only want one character as reply 2, type "? ", so it gets picked up correctly.

g_dialogue[0] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("Greetings!", "I'm the story-slime.. I will tell you", "a story you might enjoy..", "Ok..", " ")
			elseif getResponse() == OK then
				iter = advance(Stay, iter + 1) -- advance to next dialog and stay
			end
		elseif iter == 2 then
			if not g_wait_response then
				disp_dw("I know all the juicy bits about this very land!", "Interested?", " ", "Tell me more", "Please leave")
			elseif getResponse() == NO then
				iter = advance(Explore, 2)
			elseif getResponse() == YES then
				iter = advance(Stay, iter + 1) -- advance to next dialog and don't change state (stay)
			end
		elseif iter == 3 then
			if not g_wait_response then
				disp_dw("But first you have to go through some hoops..", "To prove you level of commitment.", "Does it sound good?", "Yes.", "Absoluteny not!")
			elseif getResponse() == YES then
				iter = advance(Stay, iter + 1)
			elseif getResponse() == NO then
				iter = advance(Stay, iter + 2) -- advance 2 dialog steps and stay
			end
		elseif iter == 4 then
			if not g_wait_response then
				disp_dw("Splendid, the first couple of races", "aren't hard at all", "You don't even have to steer!", "Ok great!", "Can I go?")
			elseif getResponse() == YES or getResponse() == NO then
				iter = advance(Explore, 6)
			end
		elseif iter == 5 then
			if not g_wait_response then
				disp_dw("I can't believe ou answered no to that!", "Doesn't really matter though..", "YOU MUST STILL DO IT.", "Ok ok..", " ")
			elseif getResponse() == OK then
				iter = advance(Stay, 4)
			end
		elseif iter == 6 then
			if not g_wait_response then
				disp_dw("Please go ahead and race for a bit.", "I'm gonna have tea.", " ", "Yes sir.", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 6) -- stay at this option and go to Explore state
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[1] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("There was a guy that looked like you", "that used to come here in the 70's", "But he dissapeared! He is most mysterious.", "What happened?", " ")
			elseif getResponse() == OK then
				iter = advance(Stay, iter + 1) -- advance to next dialog and go to Explore state
			end
		elseif iter == 2 then
			if not g_wait_response then
				disp_dw("He was also racing robots in this very realm.", "A master maintainer he was. I bet you could", "learn a thing or two from him.", "I'm not that bad", "You're right")
			elseif getResponse() == NO then
				iter = advance(Stay, iter + 1)
			elseif getResponse() == YES then
				iter = advance(Stay, iter + 1) -- advance to next dialog and don't change state
			end
		elseif iter == 3 then
			if not g_wait_response then
				disp_dw("Tell me. Do you think you will get out of here?", "Or will you be forever trapped, like that poor..", "..old man. ", "I wanna get out", "Probably trapped")
			elseif getResponse() == YES then
				iter = advance(Stay, iter + 1)
			elseif getResponse() == NO then
				iter = advance(Stay, iter + 2) -- advance 2 dialog steps and stay
			end
		elseif iter == 4 then
			if not g_wait_response then
				disp_dw("Hah! The ignorance! You will surely die lol.", "But first you will go mental.", "It's standard procedure.", "Yikes!", "(Try to leave)")
			elseif getResponse() == YES or getResponse() == NO then
				iter = advance(Stay, iter + 1)
			end
		elseif iter == 5 then
			if not g_wait_response then
				disp_dw("Yeeess. Trapped forever and slowly..", "..going completely mental.. BTW", "that horse builds state of the art robots!", "Will visit horse.", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 6) -- reset dialogue and go to Explore state
			end
		else
			if not g_wait_response then
				disp_dw("I have nothing more to tell you at this time..", "But really, that horse is fantastic.", "Makes incredible stuff. Great guy too.", "Ok.", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 6)
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[2] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("How did this place come about? I hear you ask.", "Slimes have been running the show since forever!", "We mess with people and get them trapped!", "That's terrible", " ")
			elseif getResponse() == OK then
				iter = advance(Stay, iter + 1) -- advance to next dialog and go to Explore state
			end
		elseif iter == 2 then
			if not g_wait_response then
				disp_dw("We also make use of these robots we found.", "We saved them, you could say.", "We really are good people doing gods work.", "You're insane.", "You do seem nice.")
			elseif getResponse() == NO then
				iter = advance(Stay, iter + 2)
			elseif getResponse() == YES then
				iter = advance(Stay, iter + 1) -- advance to next dialog and don't change state
			end
		elseif iter == 3 then
			if not g_wait_response then
				disp_dw("YOU'RE insane! Coming at me with an attitude.", "Have I told you what happened to the..", "..old man? ", "Yes you did", "Here we go.")
			elseif getResponse() == YES then
				iter = advance(Stay, iter + 2)
			elseif getResponse() == NO then
				iter = advance(Stay, iter + 2) -- advance 2 dialog steps and stay
			end
		elseif iter == 4 then
			if not g_wait_response then
				disp_dw("Thanks so much! I've been through alot lately.", "Have I told you what happened to the..", "..old man? ", "Yes you did", "Here we go.")
			elseif getResponse() == YES then
				iter = advance(Stay, iter + 1)
			elseif getResponse() == NO then
				iter = advance(Stay, iter + 1) -- advance 2 dialog steps and stay
			end
		elseif iter == 5 then
			if not g_wait_response then
				disp_dw("Well I will tell you again!", "He went mental. And dissapeared.", " ", "Well said..", "(Try to leave)")
			elseif getResponse() == YES or getResponse() == NO then
				iter = advance(Stay, iter + 1)
			end
		elseif iter == 6 then
			if not g_wait_response then
				disp_dw("Forever.. trapped.. and slowly..", "..going.. ..mental..", " ", "You have been great.", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 7) -- reset dialogue and go to Explore state
			end
		else
			if not g_wait_response then
				disp_dw("Phew, that little rant got me tired.", "I got nothing.", " ", "Ok.", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 7)
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[3] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("This is my house.", "Please do not disturb me at this time", " ", "Ok.", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 1)
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[20] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("Hello! I am the info-slime", "I will inform you about..", "..things...", "Good stuff..", " ")
			elseif getResponse() == OK then
				iter = advance(Stay, iter + 1)
			end
		elseif iter == 2 then
			if not g_wait_response then
				disp_dw("Just tap where you want to go on the map.", "Use the menu button up left (or ESC) to check", "your stats and robot collection.", "Tell me more", "Ok bye")
			elseif getResponse() == NO then
				iter = advance(Explore, 2)
			elseif getResponse() == YES then
				iter = advance(Stay, iter + 1) -- stay advance to next dialog
			end
		elseif iter == 3 then
			if not g_wait_response then
				disp_dw("I have nothing more to tell you!", "Go and find other, wiser info slimes.. heh..", " ", "Ok", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 2) -- go to explore and stay at this option
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[21] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("Info-slimes are obsessed with MAINTENANCE!", "It will target the lowest level robot", "in your collection. And improve it.", "Good stuff..", " ")
			elseif getResponse() == OK then
				iter = advance(Stay, iter + 1) -- reset dialogue and go to Explore state You can even use arrow and QWE, ASD-keys!
			end
		elseif iter == 2 then
			if not g_wait_response then
				disp_dw("If you succeed you get XP and cash!", "It's a risk reward deal.. Just don't fail..", "Wanna try?", "Lets do it", "No way")
			elseif getResponse() == NO then
				iter = advance(Explore, 1)
			elseif getResponse() == YES then
				iter = advance(Maintenance, 1) -- run maintenance go to first
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[22] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("There are 2 basic race types. Capture Flags (CF)", "And Death Match (DM)", "You should really come prepared for the DM.", "What else?", " ")
			elseif getResponse() == OK then
				iter = advance(Stay, iter + 1)
			end
		elseif iter == 2 then
			if not g_wait_response then
				disp_dw("But seriously, it's maintenance time again.", "It's harder this time.", "Wanna try?", "Lets do it", "No thanks.")
			elseif getResponse() == NO then
				iter = advance(Explore, 1)
			elseif getResponse() == YES then
				iter = advance(Maintenance, iter + 1) -- run maintenance and advance to next dialog
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[23] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("Lasers and Shields are a must these days.", "But if you can get you hands on ROBOT-", "CONTROL! You'll get total control of you robot!", "Wow!", " ")
			elseif getResponse() == OK then
				iter = advance(Stay, iter + 1)
			end
		elseif iter == 2 then
			if not g_wait_response then
				disp_dw("You can equip the addons in the menu.", "And use them in the race by pushing the buttons..", "You can even use the keyboard! (ASZX)", "Ok", " ")
			elseif getResponse() == OK then
				iter = advance(Stay, iter + 1)
			end
		elseif iter == 3 then
			if not g_wait_response then
				disp_dw("Anyway, wanna do some maintenance?", "Please?", " ", "Lets do it.", "No thanks.")
			elseif getResponse() == NO then
				iter = advance(Explore, 1)
			elseif getResponse() == YES then
				iter = advance(Maintenance, iter + 1) -- run maintenance and advance to next dialog
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[24] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("Maintenance time!", "It's optional but highly recommended.", " ", "Yes please", "No please :)")
			elseif getResponse() == YES then
				iter = advance(Maintenance, 1)
			elseif getResponse() == NO then
				iter = advance(Explore, 1) -- reset and go to explore
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[25] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("We're having so much fun maintaining.", "Please do this optional one, just this time.", " ", "Ok ok.", "Never!")
			elseif getResponse() == YES then
				iter = advance(Maintenance, 1)
			elseif getResponse() == NO then
				iter = advance(Explore, 1) -- reset and go to explore
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[26] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("FORCED MAINTENANCE!", "I love these!", " ", "I have no choice?", " ")
			elseif getResponse() == OK then
				iter = advance(Maintenance, 1)
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[40] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("At the start, pick the corner with the", "closest distance to the flag in the center!,", "Just click the corner you want. Be quick.", "Let's race", "I don't wanna")
			elseif getResponse() == YES then
				iter = advance(Race, 1)
			elseif getResponse() == NO then
				iter = advance(Explore, 1) -- reset and go to explore
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[41] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("These beginning races are really easy.", "But I assure you, you will hate me soon enough.", "Wanna race? Just capture the flag first.", "Yes", "No")
			elseif getResponse() == YES then
				iter = advance(Race, 1)
			elseif getResponse() == NO then
				iter = advance(Explore, 1) -- reset and go to explore
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[42] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("If a robot dies in a race it's forever lost.", "This is the harsh reality of our business.", "Still wanna race?", "Yes", "No")
			elseif getResponse() == YES then
				iter = advance(Race, 1)
			elseif getResponse() == NO then
				iter = advance(Explore, 1) -- reset and go to explore
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[43] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("Racing time!", "Do you want to?", " ", "Yes", "No")
			elseif getResponse() == YES then
				iter = advance(Race, 1)
			elseif getResponse() == NO then
				iter = advance(Explore, 1) -- reset and go to explore
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[90] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				local result = string.format("It seems to be %s!", giveRobot())
				openChest()
				disp_dw("You found a robot!", result, " ", "That's great", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 2)
			end
		elseif iter == 2 then
			if not g_wait_response then
				disp_dw("It's empty.", " ", " ", "Ok", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 2) -- go to explore and stay at this option
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[91] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				local result = string.format("You found %.0f coins!", giveFunds())
				openChest()
				disp_dw(result, " ", " ", "Nice", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 2)
			end
		elseif iter == 2 then
			if not g_wait_response then
				disp_dw("It's empty.", " ", " ", "Ok", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 2) -- go to explore and stay at this option
			end
		end

		coroutine.yield()
		
	end
end)

g_dialogue[92] = coroutine.create(function ()
    local iter = 1
    local chance = 1
    while true do
        if iter == 1 then
            if not g_wait_response then
                disp_dw("A chest out in the open!", "Surely it's not a trap..", "What's the worst that could happen?", "Take a chance", "Leave it")
            elseif getResponse() == YES then
                iter = advance(Stay, 2)
            elseif getResponse() == NO then
                iter = advance(Explore, 1)
            end
        elseif iter == 2 then
            if not g_wait_response then
                chance = math.random(1, 3)
                if chance < 2 then
                    disp_dw("It's a trap! You have to do maintenance now.", " ", " ", "I knew it.", " ")
                else
                    local result = string.format("You found %.0f coins!", giveFunds())
                    openChest()
                    disp_dw(result, " ", " ", "Great!", " ")
                end
            elseif getResponse() == OK then
                if chance < 2 then
                    iter = advance(Maintenance, 3)
                else
                    iter = advance(Explore, 3)
                end
            end
        elseif iter == 3 then
            if not g_wait_response then
                disp_dw("It's empty.", " ", " ", "Ok", " ")
            elseif getResponse() == OK then
                iter = advance(Explore, 2) -- go to explore and stay at this option
            end
        end

        coroutine.yield()
    end
end)


g_dialogue[93] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				local result = string.format("It seems to be %s!", giveRobot())
				openChest()
				disp_dw("You found a robot!", result, " ", "That's great", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 2)
			end
		elseif iter == 2 then
			if not g_wait_response then
				disp_dw("It's empty.", " ", " ", "Ok", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 2) -- go to explore and stay at this option
			end
		end

		coroutine.yield()
		
	end
end)


g_dialogue[94] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("This rock is loose.. Should I push it away?", " ", " ", "Yes", "No")
			elseif getResponse() == YES then
				iter = advance(Stay, iter + 1)
			elseif getResponse() == NO then
				iter = advance(Explore, 1)
			end
		elseif iter == 2 then
			if not g_wait_response then
				local result = string.format("It seems to be %s!", giveRobot())
				openChest()
				disp_dw("It's a chest behind it!", "You open it.. It's a robot!", result, "That's great", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 3)
			end
		elseif iter == 3 then
			if not g_wait_response then
				disp_dw("It's empty.", " ", " ", "Ok", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 3) -- go to explore and stay at this option
			end
		end

		coroutine.yield()
		
	end
end)


g_dialogue[110] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("The horse stares at you with chilling eyes.", "You try to look away but it forces you", "back into its devilish gaze.", "...", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 2)
			end
		elseif iter == 2 then
			if not g_wait_response then
				local result = string.format("It seems to be %s!", giveRobot())
				openChest()
				disp_dw("Take this robot! (Says the horse)", result, " ", "That's great", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 3)
			end
		elseif iter == 3 then
			if not g_wait_response then
				disp_dw("The horse seems uninterested.", " ", " ", "What was that about?", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 3) -- go to explore and stay at this option
			end
		end

		coroutine.yield()
		
	end
end)


g_dialogue[111] = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			if not g_wait_response then
				disp_dw("YOU MADE IT TO THE END OF THE BETA!", "Yes I am the old man everybody's on about.", "Great work! (Did you find the hidden chest?)", "I really did it!", " ")
			elseif getResponse() == OK then
				iter = advance(Explore, 1)
			end
		end

		coroutine.yield()
		
	end
end)


function fbl_lua_start()

	make_strict()	-- have to declare variables beforehand

	-- print("Lua dialogue system initialized!")
	
end

function fbl_lua_loop()

	local dialogue_index

	if getState() == Dialogue then

		-- get the correct dialogue id
		dialogue_index = getCurrentDialogueId()
	
		-- then resume it!
		coroutine.resume(g_dialogue[dialogue_index])
		
	end

end

function fbl_lua_end()

	print("Lua dialogue system ended!\n")
	
end
