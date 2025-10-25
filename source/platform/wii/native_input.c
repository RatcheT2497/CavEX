/*
	Copyright (c) 2022 ByteBit/xtreme8000

	This file is part of CavEX.

	CavEX is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	CavEX is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with CavEX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <wiiuse/wpad.h>
#include "../input.h"
#include "../gfx.h"

static struct {
	float dx, dy;
	float magnitude;
	bool available;
} joystick_input[3];

static bool js_emulated_btns_prev[3][4];
static bool js_emulated_btns_held[3][4];

void input_init() {
	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(WPAD_CHAN_0, gfx_width(), gfx_height());

	for(int k = 0; k < 4; k++) {
		for(int j = 0; j < 3; j++)
			js_emulated_btns_prev[j][k] = js_emulated_btns_held[j][k] = false;
	}
}

void input_poll() {
	WPAD_ScanPads();

	expansion_t e;
	WPAD_Expansion(WPAD_CHAN_0, &e);

	if(e.type == WPAD_EXP_NUNCHUK) {
		joystick_input[0].dx = sin(glm_rad(e.nunchuk.js.ang));
		joystick_input[0].dy = cos(glm_rad(e.nunchuk.js.ang));
		joystick_input[0].magnitude = e.nunchuk.js.mag;
		joystick_input[0].available = true;
	} else {
		joystick_input[0].available = false;
	}

	if(e.type == WPAD_EXP_CLASSIC) {
		joystick_input[1].dx = sin(glm_rad(e.classic.ljs.ang));
		joystick_input[1].dy = cos(glm_rad(e.classic.ljs.ang));
		joystick_input[1].magnitude = e.classic.ljs.mag;
		joystick_input[1].available = true;

		joystick_input[2].dx = sin(glm_rad(e.classic.rjs.ang));
		joystick_input[2].dy = cos(glm_rad(e.classic.rjs.ang));
		joystick_input[2].magnitude = e.classic.rjs.mag;
		joystick_input[2].available = true;
	} else {
		joystick_input[1].available = joystick_input[2].available = false;
	}

	for(int j = 0; j < 3; j++) {
		for(int k = 0; k < 4; k++) {
			js_emulated_btns_prev[j][k] = js_emulated_btns_held[j][k];
			js_emulated_btns_held[j][k] = false;
		}

		if(joystick_input[j].available) {
			float x = joystick_input[j].dx * joystick_input[j].magnitude;
			float y = joystick_input[j].dy * joystick_input[j].magnitude;

			if(x > 0.2F) {
				js_emulated_btns_held[j][3] = true;
			} else if(x < -0.2F) {
				js_emulated_btns_held[j][2] = true;
			}

			if(y > 0.2F) {
				js_emulated_btns_held[j][0] = true;
			} else if(y < -0.2F) {
				js_emulated_btns_held[j][1] = true;
			}
		}
	}
}

static uint32_t input_wpad_translate(int key) {
	switch(key) {
		case 0: return WPAD_BUTTON_UP;
		case 1: return WPAD_BUTTON_DOWN;
		case 2: return WPAD_BUTTON_LEFT;
		case 3: return WPAD_BUTTON_RIGHT;
		case 4: return WPAD_BUTTON_A;
		case 5: return WPAD_BUTTON_B;
		case 6: return WPAD_BUTTON_1;
		case 7: return WPAD_BUTTON_2;
		case 8: return WPAD_BUTTON_PLUS;
		case 9: return WPAD_BUTTON_MINUS;
		case 10: return WPAD_BUTTON_HOME;
		default: break;
	}

	expansion_t e;
	WPAD_Expansion(WPAD_CHAN_0, &e);

	if(e.type == WPAD_EXP_NUNCHUK) {
		switch(key) {
			case 100: return WPAD_NUNCHUK_BUTTON_Z;
			case 101: return WPAD_NUNCHUK_BUTTON_C;
			default: break;
		}
	} else if(e.type == WPAD_EXP_CLASSIC) {
		switch(key) {
			case 200: return WPAD_CLASSIC_BUTTON_UP;
			case 201: return WPAD_CLASSIC_BUTTON_DOWN;
			case 202: return WPAD_CLASSIC_BUTTON_LEFT;
			case 203: return WPAD_CLASSIC_BUTTON_RIGHT;
			case 204: return WPAD_CLASSIC_BUTTON_A;
			case 205: return WPAD_CLASSIC_BUTTON_B;
			case 206: return WPAD_CLASSIC_BUTTON_X;
			case 207: return WPAD_CLASSIC_BUTTON_Y;
			case 208: return WPAD_CLASSIC_BUTTON_ZL;
			case 209: return WPAD_CLASSIC_BUTTON_ZR;
			case 210: return WPAD_CLASSIC_BUTTON_FULL_L;
			case 211: return WPAD_CLASSIC_BUTTON_FULL_R;
			case 212: return WPAD_CLASSIC_BUTTON_PLUS;
			case 213: return WPAD_CLASSIC_BUTTON_MINUS;
			case 214: return WPAD_CLASSIC_BUTTON_HOME;
			default: break;
		}
	} else if(e.type == WPAD_EXP_GUITARHERO3) {
		switch(key) {
			case 300: return WPAD_GUITAR_HERO_3_BUTTON_YELLOW;
			case 301: return WPAD_GUITAR_HERO_3_BUTTON_GREEN;
			case 302: return WPAD_GUITAR_HERO_3_BUTTON_BLUE;
			case 303: return WPAD_GUITAR_HERO_3_BUTTON_RED;
			case 304: return WPAD_GUITAR_HERO_3_BUTTON_ORANGE;
			case 305: return WPAD_GUITAR_HERO_3_BUTTON_PLUS;
			case 306: return WPAD_GUITAR_HERO_3_BUTTON_MINUS;
			case 307: return WPAD_GUITAR_HERO_3_BUTTON_STRUM_UP;
			case 308: return WPAD_GUITAR_HERO_3_BUTTON_STRUM_DOWN;
			default: break;
		}
	}

	return 0;
}

void input_native_key_status(int key, bool* pressed, bool* released,
							 bool* held) {
	if(key >= 900 && key < 924) {
		int js = (key - 900) / 10;
		int offset = (key - 900) % 10;
		if(offset < 4) {
			*held = js_emulated_btns_held[js][offset]
				&& js_emulated_btns_prev[js][offset];
			*pressed = js_emulated_btns_held[js][offset]
				&& !js_emulated_btns_prev[js][offset];
			*released = !js_emulated_btns_held[js][offset]
				&& js_emulated_btns_prev[js][offset];
			return;
		}
	}

	*pressed = WPAD_ButtonsDown(WPAD_CHAN_0) & input_wpad_translate(key);
	*released = WPAD_ButtonsUp(WPAD_CHAN_0) & input_wpad_translate(key);
	*held = !(*pressed) && !(*released)
		&& WPAD_ButtonsHeld(WPAD_CHAN_0) & input_wpad_translate(key);
}

bool input_native_key_symbol(int key, int* symbol, int* symbol_help,
							 enum input_category* category, int* priority) {
	if(key >= 900 && key < 904) {
		*symbol = *symbol_help = 17;
		*category = INPUT_CAT_NUNCHUK;
		*priority = 1;
		return true;
	}

	if(key >= 910 && key < 914) {
		*symbol = *symbol_help = 18;
		*category = INPUT_CAT_CLASSIC_CONTROLLER;
		*priority = 1;
		return true;
	}

	if(key >= 920 && key < 924) {
		*symbol = *symbol_help = 19;
		*category = INPUT_CAT_CLASSIC_CONTROLLER;
		*priority = 1;
		return true;
	}

	if(key < 0 || key > 308)
		return false;

	int symbols[] = {
		[0] = 25,	[1] = 26,	[2] = 27,	[3] = 28,	[4] = 0,	[5] = 1,
		[6] = 2,	[7] = 3,	[8] = 5,	[9] = 6,	[10] = 4,	[100] = 8,
		[101] = 9,	[200] = 25, [201] = 26, [202] = 27, [203] = 28, [204] = 10,
		[205] = 11, [206] = 12, [207] = 13, [208] = 14, [209] = 15, [210] = 22,
		[211] = 23, [212] = 5,	[213] = 6,	[214] = 4,	[300] = 7,	[301] = 7,
		[302] = 7,	[303] = 7,	[304] = 7,	[305] = 5,	[306] = 6,	[307] = 7,
		[308] = 7,
	};

	*category = INPUT_CAT_NONE;

	if(key >= 0 && key <= 10)
		*category = INPUT_CAT_WIIMOTE;

	if(key >= 100 && key <= 101)
		*category = INPUT_CAT_NUNCHUK;

	if(key >= 200 && key <= 214)
		*category = INPUT_CAT_CLASSIC_CONTROLLER;

	*symbol = symbols[key];
	*symbol_help = symbols[key];

	if(*symbol_help >= 25 && *symbol_help <= 28)
		*symbol_help = 24;

	expansion_t e;
	WPAD_Expansion(WPAD_CHAN_0, &e);

	if((*category == INPUT_CAT_NUNCHUK && e.type == WPAD_EXP_NUNCHUK)
	   || (*category == INPUT_CAT_CLASSIC_CONTROLLER
		   && e.type == WPAD_EXP_CLASSIC)) {
		*priority = 2;
	} else {
		*priority = 1;
	}

	return true;
}

bool input_native_key_any(int* key) {
	return false;
}

void input_pointer_enable(bool enable) { }

bool input_pointer(float* x, float* y, float* angle) {
	struct ir_t ir;
	WPAD_IR(WPAD_CHAN_0, &ir);
	*x = ir.x;
	*y = ir.y;
	*angle = ir.angle;
	return ir.valid;
}

void input_native_joystick(float dt, float* dx, float* dy) {
	if(joystick_input[0].available && joystick_input[0].magnitude > 0.1F) {
		*dx = joystick_input[0].dx * joystick_input[0].magnitude * dt;
		*dy = joystick_input[0].dy * joystick_input[0].magnitude * dt;
	} else if(joystick_input[2].available
			  && joystick_input[2].magnitude > 0.1F) {
		*dx = joystick_input[2].dx * joystick_input[2].magnitude * dt;
		*dy = joystick_input[2].dy * joystick_input[2].magnitude * dt;
	} else {
		*dx = 0.0F;
		*dy = 0.0F;
	}
}
