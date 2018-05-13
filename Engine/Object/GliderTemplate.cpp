
#include "GliderTemplate.h"
#include "../App/File.h"

#define FILE_NAME_GLIDER_TEMPLATE_DATA	"Data/Gliders.json"

bool GliderTemplate::create(const string &name)
{
	setName(name);

	json dataModel = data(name);
	if (dataModel.empty())
		return false;

	/*"base": {
		"maxHeight": 0.7,
		"minHeight": 0.6,
		"speedHeight": 0.0025,
		"speed": 0.25,
		"speedRotate": 0.05,
		"maxSpeed": 0.25
	}*/

	maxHeight =		dataModel["maxHeight"].is_number_float()	? dataModel["maxHeight"].get<float>()	: 0.7f;
	minHeight =		dataModel["minHeight"].is_number_float()	? dataModel["minHeight"].get<float>()	: 0.76;
	speedHeight =	dataModel["speedHeight"].is_number_float()	? dataModel["speedHeight"].get<float>()	: 0.0025f;
	speed =			dataModel["speed"].is_number_float()		? dataModel["speed"].get<float>()		: 0.25f;
	speedRotate =	dataModel["speedRotate"].is_number_float()	? dataModel["speedRotate"].get<float>()	: 0.05f;
	maxSpeed =		dataModel["maxSpeed"].is_number_float()		? dataModel["maxSpeed"].get<float>()	: 0.25f;

	return true;
}

// STATIC

json GliderTemplate::_data;

json GliderTemplate::data(const string &name)
{
	if (_data.empty())
	{
		char *dataString = File::loadText(FILE_NAME_GLIDER_TEMPLATE_DATA);
		if (!dataString) return json::basic_json();

		_data = json::parse(dataString);
	}

	return _data[name].is_structured() ? _data[name] : json::basic_json();
}

void GliderTemplate::removeData()
{
	_data.clear();
}
