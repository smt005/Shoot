#include "App.h"

#include <Windows.h>
#include "GL/glew.h"
#include "glfw3.h"

#include <string>

#include "../Game/Game.h"

using namespace std;

struct AppConfig
{
	int windowWidth = 1024;
	int windowHeight = 512;
	const string name = ("Shoot");
};

AppConfig appConfig;

int App::execution()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(appConfig.windowWidth, appConfig.windowHeight, appConfig.name.c_str(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetMouseButtonCallback(window, App::mouseButtonCallback);
	glfwSetCursorPosCallback(window, App::cursorPositionCallback);
	glfwSetKeyCallback(window, App::keyCallback);

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		return -1;

	Game* game = Game::getGame();
	game->init();

	while (!glfwWindowShouldClose(window))
	{
		actionOnFrame();

		game->onFrame();
		game->draw();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


void App::actionOnFrame()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	float pos[] = { static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };
	//Callback::move(pos);
	//Callback::tap_pinch();
}

int App::width()
{
	int width = appConfig.windowWidth;
	// ...
	return width != 0 ? width : 1;
}

int App::height()
{
	int height = appConfig.windowHeight;
	// ...
	return height != 0 ? height : 1;
}

float App::aspect()
{
	int width = appConfig.windowWidth;
	int height = appConfig.windowHeight;

	// ...

	return height > 0 ? static_cast<float>(width) / static_cast<float>(height) : 1.0f;
}

double App::getCurentTime()
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	int currentTime = (systemTime.wMinute * 60000) + (systemTime.wSecond * 1000) + systemTime.wMilliseconds;
	return static_cast<double>(currentTime);
}

void App::close()
{
	//Game::gameSave();
	exit(1);
}

//-----------------

void App::cursorPositionCallback(GLFWwindow* Window, double x, double y)
{

}

void App::mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods)
{
	switch (Action)
	{
	case GLFW_PRESS:
	{
		switch (Button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
		{
			//Callback::tap_down();
		}
		break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
		{

		}
		break;
		case GLFW_MOUSE_BUTTON_RIGHT:
		{

		}
		break;
		}
	}
	break;

	case GLFW_RELEASE:
	{
		switch (Button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
		{
			//Callback::tap_up();
		}
		break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
		{

		}
		break;
		case GLFW_MOUSE_BUTTON_RIGHT:
		{

		}
		break;
		}
	}
	break;
	}
}

void App::keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
	switch (Action)
	{
	case GLFW_PRESS:
	{
		//Callback::buttonDown(Key);
	}
	break;

	case GLFW_RELEASE:
	{
		//Callback::buttonUp(Key);
	}
	break;
	}
}

inline float aspect();