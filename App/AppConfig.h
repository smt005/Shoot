#pragma once

#include <string>

using namespace std;

class AppConfig
{
private:
	int _width = 640;
	int _height = 480;
	bool _resolutionmonitor = true;
	bool _fullscreen = false;

	string _resourcesDir;
	string _nameGame = "GameTerrain";

public:
	int getHeight() { return _height; };
	int getWidth() { return _width; };
	bool hasResolutionmonitor() { return _resolutionmonitor; }
	bool hasFullscreen() { return _fullscreen; };

	const string& getResourcesDir() { return _resourcesDir; };
	const string& getNameGame() { return _nameGame; };

public:
	void load();
	void save();
};
