#pragma once

struct GLFWwindow;
class AppConfig;

class App final
{
public:
	static int execution(const char* exeFile);
	
	static double getCurentTime();
	static void close();

	static AppConfig& getAppConfig();

	static inline int width() { return _width; };
	static inline int height() { return _height; };
	static inline float aspect() { return _aspect; };

private:
	static void actionOnFrame();

	static void cursorPositionCallback(GLFWwindow* Window, double x, double y);
	static void mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods);
	static void keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);

private:
	static AppConfig _appConfig;
	static GLFWwindow* _window;
	static int _width;
	static int _height;
	static float _aspect;
};
