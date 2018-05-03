#pragma once

struct GLFWwindow;

class App final
{
public:
	static int execution();
	static void actionOnFrame();

	static double getCurentTime();
	static void close();

	static inline int width();
	static inline int height();
	static inline float aspect();

	static void cursorPositionCallback(GLFWwindow* Window, double x, double y);
	static void mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods);
	static void keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);
};
