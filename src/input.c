/*
 *
 *	FBL ENGINE
 *
 *	input.c
 *
 *	Contains input related functions including keyboard, joystick,
 *  mouse and touch support.
 *
 *	Hans Strömquist 2017 - 2022
 *
 */

#include "engine.h"

#define NUM_KEYS 69
#define NUM_MOUSE_BUTTONS 2


bool key_down[NUM_KEYS] = { 0 };
bool key_up[NUM_KEYS] = { 0 };

bool mouse_button_down[NUM_MOUSE_BUTTONS] = { 0 };
bool mouse_button_up[NUM_MOUSE_BUTTONS] = { 0 };

int logical_mouse_x = 0;
int logical_mouse_y = 0;

int last_touch_x = 0;
int last_touch_y = 0;

/*
 * Get key by returning the bool at the appropriate index.
 */
bool fbl_get_key_down(int key)
{

	return key_down[key];

}

bool fbl_get_key_up(int key)
{

	return key_up[key];

}

bool fbl_get_mouse_click(int button)
{

	return mouse_button_down[button];

}

bool fbl_get_mouse_release(int button)
{
	
	return mouse_button_up[button];

}


/*
 * Get mouse and touch coordinates.
 */
int fbl_get_mouse_x()
{

	int x, y;

	SDL_GetMouseState(&x, &y);

	/* if on browser on phone or tablet */

	if(x == 0 && y == 0)
		x = last_touch_x;

	return x;
	
}

int fbl_get_mouse_y()
{

	int x, y;

	SDL_GetMouseState(&x, &y);

	/* if on browser on phone or tablet */

	if (x == 0 && y == 0)
		y = last_touch_y;

	return y;
	
}

/*
 * Get mouse and touch coordinates when using logical resolution.
 */
int fbl_get_mouse_logical_x()
{

	return logical_mouse_x;

}

int fbl_get_mouse_logical_y()
{

	return logical_mouse_y;

}

/*
 * Deal with all keypresses
 */
void engine_process_keyboard(SDL_Event *event)
{

	int i;

	if(fbl_get_raw_frames_count() % 10 == 0) for (i = 0; i < NUM_KEYS; i++) key_up[i] = false;	// stops the key up event to keep firing.. (is this unnecessarily slow?)

	if(event->key.type == SDL_KEYDOWN)
	{

		switch (event->key.keysym.sym)
		{

			case SDLK_UP :
				key_down[0] = true;
			break;
			case SDLK_DOWN :
				key_down[1] = true;
			break;
			case SDLK_LEFT :
				key_down[2] = true;
			break;
			case SDLK_RIGHT :
				key_down[3] = true;
			break;

			case SDLK_SPACE :
				key_down[4] = true;
			break;
			case SDLK_RETURN :
				key_down[5] = true;
			break;
			case SDLK_ESCAPE :
				key_down[6] = true;
			break;
			case SDLK_BACKSPACE :
				key_down[7] = true;
			break;

			case SDLK_LCTRL:
				key_down[8] = true;
			break;
			case SDLK_LSHIFT:
				key_down[9] = true;
			break;
			case SDLK_LALT:
				key_down[10] = true;
			break;
			case SDLK_TAB:
				key_down[11] = true;
			break;

			case SDLK_RCTRL:
				key_down[12] = true;
			break;
			case SDLK_RSHIFT:
				key_down[13] = true;
			break;
			case SDLK_RALT:
				key_down[14] = true;
			break;

			case SDLK_INSERT:
				key_down[15] = true;
			break;
			case SDLK_DELETE:
				key_down[16] = true;
			break;
			case SDLK_HOME:
				key_down[17] = true;
			break;
			case SDLK_END:
				key_down[18] = true;
			break;
			case SDLK_PAGEUP:
				key_down[19] = true;
			break;
			case SDLK_PAGEDOWN:
				key_down[20] = true;
			break;

			case SDLK_a :
				key_down[21] = true;
			break;
			case SDLK_b :
				key_down[22] = true;
			break;
			case SDLK_c :
				key_down[23] = true;
			break;
			case SDLK_d :
				key_down[24] = true;
			break;
			case SDLK_e :
				key_down[25] = true;
			break;
			case SDLK_f :
				key_down[26] = true;
			break;
			case SDLK_g :
				key_down[27] = true;
			break;
			case SDLK_h :
				key_down[28] = true;
			break;
			case SDLK_i :
				key_down[29] = true;
			break;
			case SDLK_j :
				key_down[30] = true;
			break;
			case SDLK_k :
				key_down[31] = true;
			break;
			case SDLK_l :
				key_down[32] = true;
			break;
			case SDLK_m :
				key_down[33] = true;
			break;
			case SDLK_n :
				key_down[34] = true;
			break;
			case SDLK_o :
				key_down[35] = true;
			break;
			case SDLK_p :
				key_down[36] = true;
			break;
			case SDLK_q :
				key_down[37] = true;
			break;
			case SDLK_r :
				key_down[38] = true;
			break;
			case SDLK_s :
				key_down[39] = true;
			break;
			case SDLK_t :
				key_down[40] = true;
			break;
			case SDLK_u :
				key_down[41] = true;
			break;
			case SDLK_v :
				key_down[42] = true;
			break;
			case SDLK_w :
				key_down[43] = true;
			break;
			case SDLK_x :
				key_down[44] = true;
			break;
			case SDLK_y :
				key_down[45] = true;
			break;
			case SDLK_z :
				key_down[46] = true;
			break;


			case SDLK_0 :
				key_down[47] = true;
			break;
			case SDLK_1 :
				key_down[48] = true;
			break;
			case SDLK_2 :
				key_down[49] = true;
			break;
			case SDLK_3 :
				key_down[50] = true;
			break;
			case SDLK_4 :
				key_down[51] = true;
			break;
			case SDLK_5 :
				key_down[52] = true;
			break;
			case SDLK_6 :
				key_down[53] = true;
			break;
			case SDLK_7 :
				key_down[54] = true;
			break;
			case SDLK_8 :
				key_down[55] = true;
			break;
			case SDLK_9 :
				key_down[56] = true;
			break;

			case SDLK_F1 :
				key_down[57] = true;
			break;
			case SDLK_F2 :
				key_down[58] = true;
			break;
			case SDLK_F3 :
				key_down[59] = true;
			break;
			case SDLK_F4 :
				key_down[60] = true;
			break;
			case SDLK_F5 :
				key_down[61] = true;
			break;
			case SDLK_F6 :
				key_down[62] = true;
			break;
			case SDLK_F7 :
				key_down[63] = true;
			break;
			case SDLK_F8 :
				key_down[64] = true;
			break;
			case SDLK_F9 :
				key_down[65] = true;
			break;
			case SDLK_F10 :
				key_down[66] = true;
			break;
			case SDLK_F11 :
				key_down[67] = true;
			break;
			case SDLK_F12 :
				key_down[68] = true;
			break;

		}

	}
	else if(event->key.type == SDL_KEYUP)
	{

		switch (event->key.keysym.sym)
		{

			case SDLK_UP:
				key_down[0] = false;
				key_up[0] = true;
			break;
			case SDLK_DOWN:
				key_down[1] = false;
				key_up[1] = true;
			break;
			case SDLK_LEFT:
				key_down[2] = false;
				key_up[2] = true;
			break;
			case SDLK_RIGHT:
				key_down[3] = false;
				key_up[3] = true;
			break;

			case SDLK_SPACE:
				key_down[4] = false;
				key_up[4] = true;
			break;
			case SDLK_RETURN:
				key_down[5] = false;
				key_up[5] = true;
			break;
			case SDLK_ESCAPE:
				key_down[6] = false;
				key_up[6] = true;
			break;
			case SDLK_BACKSPACE:
				key_down[7] = false;
				key_up[7] = true;
			break;

			case SDLK_LCTRL:
				key_down[8] = false;
				key_up[8] = true;
			break;
			case SDLK_LSHIFT:
				key_down[9] = false;
				key_up[9] = true;
			break;
			case SDLK_LALT:
				key_down[10] = false;
				key_up[10] = true;
			break;
			case SDLK_TAB:
				key_down[11] = false;
				key_up[11] = true;
			break;

			case SDLK_RCTRL:
				key_down[12] = false;
				key_up[12] = true;
			break;
			case SDLK_RSHIFT:
				key_down[13] = false;
				key_up[13] = true;
			break;
			case SDLK_RALT:
				key_down[14] = false;
				key_up[14] = true;
			break;

			case SDLK_INSERT:
				key_down[15] = false;
				key_up[15] = true;
			break;
			case SDLK_DELETE:
				key_down[16] = false;
				key_up[16] = true;
			break;
			case SDLK_HOME:
				key_down[17] = false;
				key_up[17] = true;
			break;
			case SDLK_END:
				key_down[18] = false;
				key_up[18] = true;
			break;
			case SDLK_PAGEUP:
				key_down[19] = false;
				key_up[19] = true;
			break;
			case SDLK_PAGEDOWN:
				key_down[20] = false;
				key_up[20] = true;
			break;

			case SDLK_a:
				key_down[21] = false;
				key_up[21] = true;
			break;
			case SDLK_b:
				key_down[22] = false;
				key_up[22] = true;
			break;
			case SDLK_c:
				key_down[23] = false;
				key_up[23] = true;
			break;
			case SDLK_d:
				key_down[24] = false;
				key_up[24] = true;
			break;
			case SDLK_e:
				key_down[25] = false;
				key_up[25] = true;
			break;
			case SDLK_f:
				key_down[26] = false;
				key_up[26] = true;
			break;
			case SDLK_g:
				key_down[27] = false;
				key_up[27] = true;
			break;
			case SDLK_h:
				key_down[28] = false;
				key_up[28] = true;
			break;
			case SDLK_i:
				key_down[29] = false;
				key_up[29] = true;
			break;
			case SDLK_j:
				key_down[30] = false;
				key_up[30] = true;
			break;
			case SDLK_k:
				key_down[31] = false;
				key_up[31] = true;
			break;
			case SDLK_l:
				key_down[32] = false;
				key_up[32] = true;
			break;
			case SDLK_m:
				key_down[33] = false;
				key_up[33] = true;
			break;
			case SDLK_n:
				key_down[34] = false;
				key_up[34] = true;
			break;
			case SDLK_o:
				key_down[35] = false;
				key_up[35] = true;
			break;
			case SDLK_p:
				key_down[36] = false;
				key_up[36] = true;
			break;
			case SDLK_q:
				key_down[37] = false;
				key_up[37] = true;
			break;
			case SDLK_r:
				key_down[38] = false;
				key_up[38] = true;
			break;
			case SDLK_s:
				key_down[39] = false;
				key_up[39] = true;
			break;
			case SDLK_t:
				key_down[40] = false;
				key_up[40] = true;
			break;
			case SDLK_u:
				key_down[41] = false;
				key_up[41] = true;
			break;
			case SDLK_v:
				key_down[42] = false;
				key_up[42] = true;
			break;
			case SDLK_w:
				key_down[43] = false;
				key_up[43] = true;
			break;
			case SDLK_x:
				key_down[44] = false;
				key_up[44] = true;
			break;
			case SDLK_y:
				key_down[45] = false;
				key_up[45] = true;
			break;
			case SDLK_z:
				key_down[46] = false;
				key_up[46] = true;
			break;


			case SDLK_0:
				key_down[47] = false;
				key_up[47] = true;
			break;
			case SDLK_1:
				key_down[48] = false;
				key_up[48] = true;
			break;
			case SDLK_2:
				key_down[49] = false;
				key_up[49] = true;
			break;
			case SDLK_3:
				key_down[50] = false;
				key_up[50] = true;
			break;
			case SDLK_4:
				key_down[51] = false;
				key_up[51] = true;
			break;
			case SDLK_5:
				key_down[52] = false;
				key_up[52] = true;
			break;
			case SDLK_6:
				key_down[53] = false;
				key_up[53] = true;
			break;
			case SDLK_7:
				key_down[54] = false;
				key_up[54] = true;
			break;
			case SDLK_8:
				key_down[55] = false;
				key_up[55] = true;
			break;
			case SDLK_9:
				key_down[56] = false;
				key_up[56] = true;
			break;

			case SDLK_F1:
				key_down[57] = false;
				key_up[57] = true;
			break;
			case SDLK_F2:
				key_down[58] = false;
				key_up[58] = true;
			break;
			case SDLK_F3:
				key_down[59] = false;
				key_up[59] = true;
			break;
			case SDLK_F4:
				key_down[60] = false;
				key_up[60] = true;
			break;
			case SDLK_F5:
				key_down[61] = false;
				key_up[61] = true;
			break;
			case SDLK_F6:
				key_down[62] = false;
				key_up[62] = true;
			break;
			case SDLK_F7:
				key_down[63] = false;
				key_up[63] = true;
			break;
			case SDLK_F8:
				key_down[64] = false;
				key_up[64] = true;
			break;
			case SDLK_F9:
				key_down[65] = false;
				key_up[65] = true;
			break;
			case SDLK_F10:
				key_down[66] = false;
				key_up[66] = true;
			break;
			case SDLK_F11:
				key_down[67] = false;
				key_up[67] = true;
			break;
			case SDLK_F12:
				key_down[68] = false;
				key_up[68] = true;
			break;

		}

	}

}


/*
 * Deal with all mouse buttons
 */
void engine_process_mouse(SDL_Event *event)
{

	mouse_button_up[0] = false;
	mouse_button_up[1] = false;

	if (event->type == SDL_MOUSEBUTTONDOWN)// could use event->type == SDL_FINGERDOWN aswell for touch specifics
	{

		switch (event->button.button)
		{

			case SDL_BUTTON_LEFT :
				mouse_button_down[0] = true;
			break;
			case SDL_BUTTON_RIGHT:
				mouse_button_down[1] = true;
			break;

		}

	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{

		switch (event->button.button)
		{

			case SDL_BUTTON_LEFT:
				mouse_button_down[0] = false;
				mouse_button_up[0] = true;
			break;
			case SDL_BUTTON_RIGHT:
				mouse_button_down[1] = false;
				mouse_button_up[1] = true;
			break;

		}


	}
	else if (event->type == SDL_MOUSEMOTION) {
		//printf("x=%d, y=%d, xrel=%d, yrel=%d\n", event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel);
		logical_mouse_x = event->motion.x;
		logical_mouse_y = event->motion.y;
	}

}


/* end of input.c */
