#include "AppConfig.h"

#include "../App/File.h"
#include "../Engine/Common/Json.h"

using namespace std;
using json = nlohmann::json;

void AppConfig::load()
{
	string fileName = "Data/Setting.json";
	char* dataChar = File::loadText(fileName);
	if (!dataChar)
		return;

	json data = json::parse(dataChar);
	delete[] dataChar;

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

	_height = data["height"].is_number_integer() ? data["height"].get<int>() : 480;
	_width = data["width"].is_number_integer() ? data["width"].get<int>() : 640;
	_resolutionmonitor = data["resolutionmonitor"].is_boolean() ? data["resolutionmonitor"].get<bool>() : true;
	_fullscreen = data["fullscreen"].is_boolean() ? data["fullscreen"].get<bool>() : false;

	if (!data["game"].is_null())
	{
		const string& nameGame = data["game"]["name"].is_string() ? data["game"]["name"] : "Game";
		_nameGame = nameGame;

		const string resourcesDir = data["resourcesDir"].is_string() ? data["resourcesDir"] : "Resources";
		_resourcesDir = resourcesDir;
	}
}

void AppConfig::save()
{

}
