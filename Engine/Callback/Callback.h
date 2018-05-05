#pragma once

class Callback
{
public:
	static void move(float* pos);
	static void tap_pinch();
	static void tap_down();
	static void tap_up();
	static void buttonDown(int Key);
	static void buttonUp(int Key);

};