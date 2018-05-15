#include "App.h"
#include "File.h"
#include "AppConfig.h"
#include "../Game/Game.h"
#include "../Engine/Callback/Callback.h"
#include "../Engine/Common/Log.h"

#include <Windows.h>
#include "GL/glew.h"
#include "glfw3.h"

#include <string>
using namespace std;

using namespace glm;

AppConfig App::_appConfig;

int App::_width;
int App::_height;
float App::_aspect;
GLFWwindow* App::_window;

string getPathExe(const char* exeFile);

int App::execution(const char* exeFile)
{
	if (!exeFile)
		return -1;

	if (!glfwInit())
		return -1;

	File::setAppResourcesDir(getPathExe(exeFile) + "/Resources/");
	_appConfig.load();

	if (_appConfig.hasFullscreen())
	{
		if (_appConfig.hasResolutionmonitor())
		{
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			_width = mode->width;
			_height = mode->height;
		}
		else
		{
			_width = _appConfig.getWidth();
			_height = _appConfig.getHeight();
		}

		_window = glfwCreateWindow(_width, _height, _appConfig.getNameGame().c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		_width = _appConfig.getWidth();
		_height = _appConfig.getHeight();

		_window = glfwCreateWindow(_appConfig.getWidth(), _appConfig.getHeight(), _appConfig.getNameGame().c_str(), NULL, NULL);
	}

	if (!_window)
	{
		_width = 640;
		_height = 480;

		_window = glfwCreateWindow(_width, _height, _appConfig.getNameGame().c_str(), NULL, NULL);

		if (!_window)
		{
			glfwTerminate();

			MessageBox(
				NULL,
				"Error creating the window. The program will be closed.",
				"ERROR",
				MB_OK |
				MB_DEFBUTTON1 |
				MB_ICONEXCLAMATION |
				MB_DEFAULT_DESKTOP_ONLY
			);

			return -1;
		}
		else
		{
			MessageBox(
				NULL,
				"Error creating the window. Window mode.",
				"ERROR",
				MB_OK |
				MB_DEFBUTTON1 |
				MB_ICONEXCLAMATION |
				MB_DEFAULT_DESKTOP_ONLY
			);
		}
	}

	_aspect = static_cast<float>(_width) / static_cast<float>(_height);

	glfwSetMouseButtonCallback(_window, App::mouseButtonCallback);
	glfwSetCursorPosCallback(_window, App::cursorPositionCallback);
	glfwSetKeyCallback(_window, App::keyCallback);

	glfwMakeContextCurrent(_window);

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}

	{
		int xpos, ypos;
		glfwGetWindowPos(_window, &xpos, &ypos);

		int width, height;
		glfwGetWindowSize(_window, &width, &height);

		SetCursorPos((xpos + width / 2), (ypos + height / 2));
	}

	Game* game = Game::getGame();
	game->init();

	while (!glfwWindowShouldClose(_window))
	{
		actionOnFrame();

		game->update();
		game->draw();

		glfwSwapBuffers(_window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


void App::actionOnFrame()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	

	// TODO:
	/*int xpos, ypos;
	glfwGetWindowPos(_window, &xpos, &ypos);

	int width, height;
	glfwGetWindowSize(_window, &width, &height);

	int right = xpos + width;
	int bottom = ypos + height;
	bool changePos = false;

	if (mousePos.x > right)
	{
		mousePos.x = xpos;
		changePos = true;
	}

	if (mousePos.x < xpos)
	{
		mousePos.x = right;
		changePos = true;
	}

	if (mousePos.y > bottom)
	{
		mousePos.y = ypos;
		changePos = true;
	}

	if (mousePos.y < ypos)
	{
		mousePos.y = bottom;
		changePos = true;
	}

	if (changePos)
		SetCursorPos(mousePos.x, mousePos.y);*/

	float pos[] = { static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };

	Callback::move(pos);
	Callback::tap_pinch();
	Callback::buttonPinch();
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
	Game::saveGame();
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
			Callback::tap_down();
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
			Callback::tap_up();
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
		Callback::buttonDown(Key);
	}
	break;

	case GLFW_RELEASE:
	{
		Callback::buttonUp(Key);
	}
	break;
	}
}

AppConfig& App::getAppConfig()
{
	return _appConfig;
}

vec2 App::pos()
{
	int xpos, ypos;
	glfwGetWindowPos(_window, &xpos, &ypos);

	return vec2(xpos, ypos);
}

//---

string getPathExe(const char* exeFile)
{
	string exeFileStr(exeFile);
	size_t index = exeFileStr.find_last_of('\\');
	string exePathStr;
	std::copy_n(exeFileStr.begin(), index, std::back_inserter(exePathStr));

	return exePathStr;
}