#include "AppConfig.h"

#include "../App/File.h"
#include "../Engine/Common/JsonHelp.h"

void AppConfig::load()
{
	string fileName = "Data/Setting.json";
	
	json data;
	if (!JsonHelp::load(data, fileName))
		return;

	_height = JsonHelp::getInt(data, "height", 480);
	_width = JsonHelp::getInt(data, "width", 640);

	_resolutionmonitor = JsonHelp::getBool(data, "resolutionmonitor", true);
	_fullscreen = JsonHelp::getBool(data, "fullscreen", false);


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