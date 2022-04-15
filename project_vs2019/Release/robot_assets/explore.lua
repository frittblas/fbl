--
--
--	ROBOT GAME
--
--	explore.lua
--
--	Contains exploration stuff
--
--	Hans Strömquist 2021
--
--

local g_sprite = 0

local guy = {}
local man = {}

local g_guy_sprite
local g_man_sprite



--functions

-- void init_explore()
-- void exit_explore()
-- void tick_explore()
-- int control_obj(obj, speed)


function init_explore()

	g_guy_sprite = fbl_create_sprite(0, 0, 32, 32, 0)	-- player sprite (guy)
	g_man_sprite = fbl_create_sprite(160, 0, 32, 32, 0)	-- npc sprite (man)

	guy.x = 63
	guy.y = 63

	guy.last_frame_x = 0
	guy.last_frame_y = 0

	man.x = 255
	man.y = 255

	fbl_set_sprite_xy(g_guy_sprite, guy.x, guy.y)
	fbl_set_sprite_xy(g_man_sprite, man.x, man.y)

end

function exit_explore()

	fbl_destroy_all_sprites()

end

function tick_explore()

	-- check if we need a reply in the dialogue (this hase to execute before coroutine resume to avoid da bug)
	if in_dialogue() then
		response_dialog()
	end

	-- take care of dialogue triggering
	if trigger_dialogue(g_guy_sprite, g_man_sprite) then
		-- put guy back so colission doesn't trigger again and again
		guy.x = guy.last_frame_x
		guy.y = guy.last_frame_y
		fbl_set_sprite_xy(g_guy_sprite, guy.x, guy.y)
	end

	-- store last frame's x, y
	guy.last_frame_x = guy.x
	guy.last_frame_y = guy.y

	-- move da guy
	if not in_dialogue() then
		if control_obj(guy, 2) == 1 then
			fbl_set_sprite_xy(g_guy_sprite, guy.x, guy.y)
		end
	end

	if fbl_get_key_down(FBLK_BACKSPACE) > 0 then		-- backspace to quit to title
		change_game_state(STATE_TITLE)
	end

end

function control_obj(obj, speed)

	local retval = 0

	if fbl_get_key_down(FBLK_W) > 0 then obj.y = obj.y - speed; retval = 1 end
	if fbl_get_key_down(FBLK_S) > 0 then obj.y = obj.y + speed; retval = 1 end
	if fbl_get_key_down(FBLK_A) > 0 then obj.x = obj.x - speed; retval = 1 end
	if fbl_get_key_down(FBLK_D) > 0 then obj.x = obj.x + speed; retval = 1 end

	return retval

end

