
#include "GliderTemplate.h"
#include "../App/File.h"

#define FILE_NAME_GLIDER_TEMPLATE_DATA	"Data/Gliders.json"

bool GliderTemplate::create(const string &name)
{
	setName(name);

	json temp = data(name);
	if (temp.empty())
		return false;

	/*"base": {
		"model": "Glider",
		"maxHeight": 0.7,
		"minHeight": 0.6,
		"speedHeight": 0.0025,
		"speed": 0.25,
		"speedRotate": 0.05,
		"maxSpeed": 0.25
	}*/

	const string& model_ = temp["model"].is_string() ? temp["model"] : "base";
	model = model_;

	maxHeight =		temp["maxHeight"].is_number_float()		? temp["maxHeight"].get<float>()	: 0.7f;
	minHeight =		temp["minHeight"].is_number_float()		? temp["minHeight"].get<float>()	: 0.76;
	speedHeight =	temp["speedHeight"].is_number_float()	? temp["speedHeight"].get<float>()	: 0.0025f;
	speed =			temp["speed"].is_number_float()			? temp["speed"].get<float>()		: 0.25f;
	speedRotate =	temp["speedRotate"].is_number_float()	? temp["speedRotate"].get<float>()	: 0.05f;
	maxSpeed =		temp["maxSpeed"].is_number_float()		? temp["maxSpeed"].get<float>()		: 0.25f;

	return true;
}

// STATIC

json GliderTemplate::_data;

json GliderTemplate::data(const string &name)
{
	if (_data.empty())
	{
		char *dataString = File::loadText(FILE_NAME_GLIDER_TEMPLATE_DATA);
		if (!dataString)
		{
			return json::basic_json();
		}

		_data = json::parse(dataString);
	}

	return _data[name].is_structured() ? _data[name] : json::basic_json();
}

void GliderTemplate::removeData()
{
	_data.clear();
}
