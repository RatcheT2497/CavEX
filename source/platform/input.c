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

#include "../cglm/cglm.h"
#include "gfx.h"
#include "input.h"
#include "../game/game_state.h"

static const char* input_config_translate(enum input_button key) {
	switch(key) {
		case IB_ACTION1: return "input.item_action_left";
		case IB_ACTION2: return "input.item_action_right";
		case IB_FORWARD: return "input.player_forward";
		case IB_BACKWARD: return "input.player_backward";
		case IB_LEFT: return "input.player_left";
		case IB_RIGHT: return "input.player_right";
		case IB_JUMP: return "input.player_jump";
		case IB_SNEAK: return "input.player_sneak";
		case IB_INVENTORY: return "input.inventory";
		case IB_HOME: return "input.open_menu";
		case IB_SCROLL_LEFT: return "input.scroll_left";
		case IB_SCROLL_RIGHT: return "input.scroll_right";
		case IB_GUI_UP: return "input.gui_up";
		case IB_GUI_DOWN: return "input.gui_down";
		case IB_GUI_LEFT: return "input.gui_left";
		case IB_GUI_RIGHT: return "input.gui_right";
		case IB_GUI_CLICK: return "input.gui_click";
		case IB_GUI_CLICK_ALT: return "input.gui_click_alt";
		case IB_SCREENSHOT: return "input.screenshot";
		default: return NULL;
	}
}

bool input_symbol(enum input_button b, int* symbol, int* symbol_help,
				  enum input_category* category) {
	const char* key = input_config_translate(b);

	if(!key)
		return false;

	size_t length = 8;
	int mapping[length];

	if(!config_read_int_array(&gstate.config_user, input_config_translate(b),
							  mapping, &length))
		return false;

	int priority = 0;
	bool has_any = false;

	for(size_t k = 0; k < length; k++) {
		int symbol_tmp, symbol_help_tmp, priority_tmp;
		enum input_category category_tmp;
		if(input_native_key_symbol(mapping[k], &symbol_tmp, &symbol_help_tmp,
								   &category_tmp, &priority_tmp)
		   && priority_tmp > priority) {
			priority = priority_tmp;
			*symbol = symbol_tmp;
			*symbol_help = symbol_help_tmp;
			*category = category_tmp;
			has_any = true;
		}
	}

	return has_any;
}

bool input_pressed(enum input_button b) {
	const char* key = input_config_translate(b);

	if(!key)
		return false;

	size_t length = 8;
	int mapping[length];

	if(!config_read_int_array(&gstate.config_user, input_config_translate(b),
							  mapping, &length))
		return false;

	bool any_pressed = false;
	bool any_held = false;
	bool any_released = false;

	for(size_t k = 0; k < length; k++) {
		bool pressed, released, held;
		input_native_key_status(mapping[k], &pressed, &released, &held);
		if(pressed)
			any_pressed = true;
		if(released)
			any_released = true;
		if(held)
			any_held = true;
	}

	return any_pressed && !any_held && !any_released;
}

bool input_released(enum input_button b) {
	const char* key = input_config_translate(b);

	if(!key)
		return false;

	size_t length = 8;
	int mapping[length];

	if(!config_read_int_array(&gstate.config_user, input_config_translate(b),
							  mapping, &length))
		return false;

	bool any_pressed = false;
	bool any_held = false;
	bool any_released = false;

	for(size_t k = 0; k < length; k++) {
		bool pressed, released, held;
		input_native_key_status(mapping[k], &pressed, &released, &held);
		if(pressed)
			any_pressed = true;
		if(released)
			any_released = true;
		if(held)
			any_held = true;
	}

	return !any_pressed && !any_held && any_released;
}

bool input_held(enum input_button b) {
	const char* key = input_config_translate(b);

	if(!key)
		return false;

	size_t length = 8;
	int mapping[length];

	if(!config_read_int_array(&gstate.config_user, input_config_translate(b),
							  mapping, &length))
		return false;

	bool any_pressed = false;
	bool any_held = false;

	for(size_t k = 0; k < length; k++) {
		bool pressed, released, held;
		input_native_key_status(mapping[k], &pressed, &released, &held);
		if(pressed)
			any_pressed = true;
		if(held)
			any_held = true;
	}

	return any_pressed || any_held;
}

bool input_joystick(float dt, float* x, float* y) {
	input_native_joystick(dt, x, y);
	return true;
}
