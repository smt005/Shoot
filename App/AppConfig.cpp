#include "AppConfig.h"

#include "../App/File.h"
#include "../Engine/Common/JsonHelp.h"

void AppConfig::load()
{
	string fileName = "Data/Setting.json";
	
	json data;
	if (!JsonHepl::load(data, fileName))
		return;

	_height = JsonHepl::getInt(data, "height", 480);
	_width = JsonHepl::getInt(data, "width", 640);

	_resolutionmonitor = JsonHepl::getBool(data, "resolutionmonitor", true);
	_fullscreen = JsonHepl::getBool(data, "fullscreen", false);


	if (!data["game"].is_null())
	{
		const string& nameGame = data["game"]["name"].is_string() ? data["game"]["name"] : "Game";
		_nameGame = nameGame;
	}
}

void AppConfig::save()
{

}

/*
{
	"width": 1280,
	"height": 1024,
	"fullscreen": true,
	"game": {
		"name": "GameTerrain"
	}
}
*/