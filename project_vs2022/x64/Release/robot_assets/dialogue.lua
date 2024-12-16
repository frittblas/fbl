--
--
--	ROBOT GAME
--
--	dialogue.lua
--
--	Contains dialogue stuff
--
--	Hans Strömquist 2021
--
--

---------------------------------
--NOTE: there's a difference between dialog (the ui-square) and dialogue (the conversation)
---------------------------------

local g_in_dialogue = false

local g_dialogue_1 = nil	-- a coroutine is later assigned to this

local g_dialog = {}	-- record of ID's to prims and text and some stuff


-- functions

-- void debug_console()
-- void init_dialogue_system()
-- void exit_dialogue_system()
-- void display_dialog()
-- void response_dialog()
-- void hide_dialog()
-- bool trigger_dialogue(is1, id2)
-- bool in_dialoge()


function debug_console(iter)

	print("g_in_dialogue = ", g_in_dialogue)

	print("g_dialog.square = ", g_dialog.square)
	print("g_dialog.text1 = ", g_dialog.text1)
	print("g_dialog.text2 = ", g_dialog.text2)
	print("g_dialog.outline = ", g_dialog.outline)
	
	print("g_dialog.yes_no = ", g_dialog.yes_no)
	print("g_dialog.response_yn = ", g_dialog.response_yn)
	print("g_dialog.response_ok = ", g_dialog.response_ok)
	print("g_dialog.ui_ok = ", g_dialog.ui_ok)
	print("g_dialog.ui_yes = ", g_dialog.ui_yes)
	print("g_dialog.ui_no = ", g_dialog.ui_no)

	-- print("g_title_text = ", g_title_text)
	print("g_get_ready_text = ", g_get_ready_text)

	print("iter = ", iter)

end

function init_dialogue_system()


	g_dialog.yes_no = false	-- if true, Yes/No alteratives will be shown, otherwise, Ok.

	-- set position and size of text area
	local x = fbl_get_screen_w() / 2
	local y = fbl_get_screen_h() / 2 + fbl_get_screen_h() / 4
	local width = 250
	local height = 100

	-- create wine-red text area
	g_dialog.square = fbl_create_prim(FBL_RECT, x, y, width, height, 0, fbl_false, fbl_true)
	fbl_set_prim_color(g_dialog.square, 200, 50, 50, 255)

	-- create white outline
	g_dialog.outline = fbl_create_prim(FBL_RECT, x, y, width, height, 0, fbl_false, fbl_false)
	fbl_set_prim_color(g_dialog.outline, 255, 255, 255, 255)

	-- create 1st line of text
	g_dialog.text1 = fbl_create_text(255, 255, 255, 0, " ")
	fbl_set_text_align(g_dialog.text1, FBL_ALIGN_LEFT)
	fbl_set_text_xy(g_dialog.text1, x - 230, y - 70)

	-- same for 2nd line
	g_dialog.text2 = fbl_create_text(255, 255, 255, 0, " ")
	fbl_set_text_align(g_dialog.text2, FBL_ALIGN_LEFT)
	fbl_set_text_xy(g_dialog.text2, x - 230, y - 30)

	-- yes/no response
	g_dialog.response_yn = fbl_create_text(255, 255, 255, 0, "    Yes         No")
	fbl_set_text_align(g_dialog.response_yn, FBL_ALIGN_LEFT)
	fbl_set_text_xy(g_dialog.response_yn, x - 215, y + 64)

	-- Ok response
	g_dialog.response_ok = fbl_create_text(255, 255, 255, 0, "    Ok")
	fbl_set_text_align(g_dialog.response_ok, FBL_ALIGN_LEFT)
	fbl_set_text_xy(g_dialog.response_ok, x - 215, y + 64)

	-- ui yes/no and Ok buttons
	g_dialog.ui_yes = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, 0)
	fbl_set_ui_elem_xy(g_dialog.ui_yes, x - 215, y + 64);
	g_dialog.ui_no = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, 0)
	fbl_set_ui_elem_xy(g_dialog.ui_no, x - 120, y + 64);
	g_dialog.ui_ok = fbl_create_ui_elem(FBL_UI_BUTTON_CLICK, 0, 0, 32, 32, 0)
	fbl_set_ui_elem_xy(g_dialog.ui_ok, x - 215, y + 64);

	hide_dialog()	-- deactivate all dialog-elements

end

function exit_dialogue_system()

	fbl_delete_text(g_dialog.text1)
	fbl_delete_text(g_dialog.text2)
	fbl_delete_text(g_dialog.response_yn)
	fbl_delete_text(g_dialog.response_ok)

	fbl_destroy_all_ui_elems()
	fbl_destroy_all_prims()

end

-- dialogues for the different characters

g_dialogue = coroutine.create(function ()
	local iter = 1
	while true do
		if iter == 1 then
			print("Hello young masta", iter)
			display_dialog("Greetings! this is the first dialog.", false)
		elseif iter == 2 then
			print("Hello old masta!! This is longer!", iter)
			display_dialog("Hello adventurer! May I insterest you in some reasonable items on your quest?", true)
		elseif iter == 3 then
			print("penultimate one", iter)
			display_dialog("if this works its fine", false)
		else
			print("last one", iter)
			display_dialog("if THIS works its nice, last message this is, im sure of it", true)
		end
		iter = iter + 1
		debug_console(iter)
		coroutine.yield()
	end
	end)


-- display the dialog by activating prims and text

function display_dialog(text, yes_no)

	-- first deactivate old dialog
	hide_dialog()

	-- activate dialog bg
	fbl_set_prim_active(g_dialog.square, fbl_true)
	fbl_set_prim_active(g_dialog.outline, fbl_true)

	-- if the text is longer than 40 chars, split into 2 lines
	if string.len(text) < 40 then

		fbl_update_text(g_dialog.text1, 255, 255, 255, 0, text)	-- update the text

		-- activate the line of text
		fbl_set_text_active(g_dialog.text1, fbl_true)
	
	else	-- 2 lines

		local line1 = string.sub(text, 1, 40)
		local tmp = string.sub(text, 41, string.len(text))
		local line2 = tmp:gsub("^%s*", "")	-- remove leading whitespace

		-- update the text, line1
		fbl_update_text(g_dialog.text1, 255, 255, 255, 0, line1)

		-- same for line2
		fbl_update_text(g_dialog.text2, 255, 255, 255, 0, line2)

		-- activate both lines
		fbl_set_text_active(g_dialog.text1, fbl_true)
		fbl_set_text_active(g_dialog.text2, fbl_true)

	end

	-- can answer yes/no
	if yes_no then
		fbl_set_text_active(g_dialog.response_yn, fbl_true)
		fbl_set_ui_elem_active(g_dialog.ui_yes, fbl_true)
		fbl_set_ui_elem_active(g_dialog.ui_no, fbl_true)
	else	-- can answer Ok
		fbl_set_text_active(g_dialog.response_ok, fbl_true)
		fbl_set_ui_elem_active(g_dialog.ui_ok, fbl_true)
	end

	g_dialog.yes_no = yes_no	-- used by response_dialog

	g_in_dialogue = true	-- currently in a dialogue (can't move character and other things)

end


-- handles the players responses

function response_dialog()

	if not g_dialog.yes_no then
		if fbl_get_ui_elem_val(g_dialog.ui_ok) == 1 then	-- clicked ok?
			print("Working OKOK!!!")
			hide_dialog()
		end
	elseif g_dialog.yes_no then
		if fbl_get_ui_elem_val(g_dialog.ui_yes) == 1 then	-- clicked yes?
			print("Working YESYES!!!")
		end
		if fbl_get_ui_elem_val(g_dialog.ui_no) == 1 then	-- clicked no?
			print("Working NONO!!!")
			hide_dialog()
		end
	end

end


-- hide dialog by deactivating all dialog elements

function hide_dialog()

	-- deactivate everything
	fbl_set_prim_active(g_dialog.square, fbl_false)
	fbl_set_prim_active(g_dialog.outline, fbl_false)

	fbl_set_text_active(g_dialog.text1, fbl_false)
	fbl_set_text_active(g_dialog.text2, fbl_false)
	fbl_set_text_active(g_dialog.response_yn, fbl_false)
	fbl_set_text_active(g_dialog.response_ok, fbl_false)

	fbl_set_ui_elem_active(g_dialog.ui_yes, fbl_false)
	fbl_set_ui_elem_active(g_dialog.ui_no, fbl_false)
	fbl_set_ui_elem_active(g_dialog.ui_ok, fbl_false)

	g_dialog.yes_no = false;

	g_in_dialogue = false

end


function trigger_dialogue(id1, id2)

	if not g_in_dialogue and fbl_get_sprite_collision(id1, id2) > 0 then
		coroutine.resume(g_dialogue)
		return true
	end

	return false

end


function in_dialogue()
	return g_in_dialogue
end