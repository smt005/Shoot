
#include "Map.h"
#include "Object.h"
#include "Glider.h"
#include "Model.h"
#include "Shell.h"
#include "../Physics/Physics.h"
#include "../../App/File.h"
#include "../Common/Help.h"

Map::~Map()
{
	help::clear(_objects);
	help::clear(_gliders);
}

bool Map::create(const string &newName)
{
	setName(newName);

	string fileName = "Map/" + name() + ".json";
	char* dataChar = File::loadText(fileName);

	if (!dataChar)
	{
		return false;
	}

	json data = json::parse(dataChar);
	delete[] dataChar;

	_area = data["area"].is_null() ? 10.0f : data["area"].get<float>();

	for (auto element : data["objects"])
	{
		const string &name = element["name"].is_string() ? element["name"] : "";
		const string &modelName = element["model"].is_string() ? element["model"] : "default";
		unsigned int type = element["type"].is_number_unsigned() ? element["type"].get<unsigned int>()  : 0;

		PhysicType physicType = PhysicType::NONE;
		const string& physicTypeString = element["physicType"].is_string() ? element["physicType"] : "none";

		if (physicTypeString == "convex")
		{
			physicType = PhysicType::CONVEX;
				
		}
		else if (physicTypeString == "terrain")
		{
			physicType = PhysicType::TERRAIN;
		}

		vec3 pos(0.0f);
		int index = 0;
		for (float elementPos : element["pos"])
		{
			pos[index] = elementPos;
			++index;
		}

		Object& object = help::add(_objects);
		object.set(name, modelName, physicType, pos);
	}

	for (auto element : data["gliders"])
	{
		const string &name = element["name"].is_string() ? element["name"] : "";
		const string &modelName = element["model"].is_string() ? element["model"] : "default";
		unsigned int type = element["type"].is_number_unsigned() ? element["type"].get<unsigned int>()  : 0;

		PhysicType physicType = PhysicType::NONE;
		const string& physicTypeString = element["physicType"].is_string() ? element["physicType"] : "none";

		if (physicTypeString == "convex")
		{
			physicType = PhysicType::CONVEX;

		}
		else if (physicTypeString == "terrain")
		{
			physicType = PhysicType::TERRAIN;
		}

		vec3 pos(0.0f);
		int index = 0;
		for (float elementPos : element["pos"])
		{
			pos[index] = elementPos;
			++index;
		}

		Glider &glider = help::add(_gliders, new Glider("base", pos, name));
	}

	return true;
}

void Map::setPhysic()
{
	/*for (auto object : _objects)
	{
		object->setPhysic();
	}*/

	//Object& loc = help::find(_objects, "MapArena");
	//loc.setPhysic();

	/*for (auto glider : _gliders)
	{
		glider->setPhysic();
	}*/
}

void Map::getDataJson(json& dataJson)
{
	dataJson["name"] = name();
	dataJson["area"] = _area;

	for (auto object : _objects)
	{
		json dataObject;
		object->getDataJson(dataObject);
		dataJson["objects"].push_back(dataObject);
	}

	for (auto glider : _gliders)
	{
		json dataObject;
		glider->getDataJson(dataObject);
		dataJson["gliders"].push_back(dataObject);
	}
}

void Map::action()
{
	for (auto object : _objects) object->action();
	for (auto glider : _gliders) glider->action();

	Shell::update();
	Physics::update();
}

Object& Map::addObjectToPos(const string& nameModel, const PhysicType& type, const glm::vec3& pos)
{
	Object &object = help::add(_objects);

	object.set("", nameModel, type, pos);
	object.setPhysic();

	return object;
}

Object& Map::addObject(const string& nameModel, const PhysicType& type, const glm::mat4x4& mat)
{
	Object& object = help::add(_objects);

	// Временно
	glm::vec3 pos = glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
	object.set("", nameModel, type, pos);
	object.setPhysic();

	return object;
}

Glider& Map::getGliderByName(const string& name)
{
	return help::find(_gliders, name);
}
