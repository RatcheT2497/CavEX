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

#include <GLFW/glfw3.h>
#include "../input.h"
#include "../gfx.h"

extern GLFWwindow* window;

static bool input_pointer_enabled;
static double input_old_pointer_x, input_old_pointer_y;
static bool input_key_held[1024];

void input_init() {
	for(int k = 0; k < 1024; k++)
		input_key_held[k] = false;

	input_pointer_enabled = false;
	input_old_pointer_x = 0;
	input_old_pointer_y = 0;
}

void input_poll() { }

void input_native_key_status(int key, bool* pressed, bool* released,
							 bool* held) {
	if(key >= 1024) {
		*pressed = false;
		*released = false;
		*held = false;
	}

	int state = key < 1000 ? glfwGetKey(window, key) :
							 glfwGetMouseButton(window, key - 1000);

	*pressed = (state == GLFW_PRESS) && !input_key_held[key];
	*released = (state == GLFW_RELEASE) && input_key_held[key];
	*held = !(*released) && input_key_held[key];

	if(state == GLFW_PRESS)
		input_key_held[key] = true;

	if(state == GLFW_RELEASE)
		input_key_held[key] = false;
}

bool input_native_key_symbol(int key, int* symbol, int* symbol_help,
							 enum input_category* category, int* priority) {
	*category = INPUT_CAT_NONE;
	*symbol = 7;
	*symbol_help = 7;
	*priority = 1;
	return true;
}

bool input_native_key_any(int* key) {
	return false;
}

void input_pointer_enable(bool enable) {
	glfwSetInputMode(window, GLFW_CURSOR,
					 enable ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

	if(!input_pointer_enabled && enable)
		glfwSetCursorPos(window, gfx_width() / 2.0, gfx_height() / 2.0);

	if(input_pointer_enabled && !enable)
		glfwGetCursorPos(window, &input_old_pointer_x, &input_old_pointer_y);

	input_pointer_enabled = enable;
}

bool input_pointer(float* x, float* y, float* angle) {
	double x2, y2;
	glfwGetCursorPos(window, &x2, &y2);
	*x = (float) x2;
	*y = (float) y2;
	*angle = 0.0F;
	return input_pointer_enabled && x2 >= 0 && y2 >= 0 && x2 < gfx_width()
		&& y2 < gfx_height();
}

void input_native_joystick(float dt, float* dx, float* dy) {
	if(!input_pointer_enabled) {
		double x2, y2;
		glfwGetCursorPos(window, &x2, &y2);
		*dx = (float) (x2 - input_old_pointer_x) * 0.001F;
		*dy = -(float) (y2 - input_old_pointer_y) * 0.001F;
		input_old_pointer_x = x2;
		input_old_pointer_y = y2;
	} else {
		*dx = 0.0F;
		*dy = 0.0F;
	}
}