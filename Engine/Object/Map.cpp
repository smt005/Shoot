
#include "Map.h"
#include "Object.h"
#include "Model.h"
#include "../Glider/Glider.h"
#include "../Glider/Shell.h"
#include "../Effect/EffectObject.h"
#include "../Physics/Physics.h"
#include "../../App/File.h"
#include "../Common/Help.h"
#include "../Common/JsonHelp.h"

Map::~Map()
{
	help::clear(_objects);
	help::clear(_gliders);
	help::clear(_effects);
}

bool Map::create(const string &newName)
{
	setName(newName);

	string fileName = "Map/" + name() + ".json";
	
	json data;
	if (!JsonHelp::load(data, fileName))
		return false;

	_area = JsonHelp::getFloat(data, "area", 1.0f);

	for (auto element : data["objects"])
	{
		const string &name = JsonHelp::getString(element, "name");
		const string &modelName = JsonHelp::getString(element, "model");

		PhysicType physicType = PhysicType::NONE;
		const string& physicTypeString = JsonHelp::getString(element, "physicType");

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
		const string &name = JsonHelp::getString(element, "name");
		const string &templateName = JsonHelp::getString(element, "template", "base");

		PhysicType physicType = PhysicType::NONE;
		const string& physicTypeString = JsonHelp::getString(data, "physicType");

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

		Glider &glider = help::add(_gliders, new Glider(templateName, pos, name));
	}

	return true;
}

void Map::setPhysic()
{
	for (auto object : _objects)
	{
		object->setPhysic();
	}

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
	// Effect
	{
		size_t countDeleteEffect = 0;
		std::remove_if(_effects.begin(), _effects.end(), [&countDeleteEffect](EffectObject* item)
		{
			if (item->_live)
			{
				return false;
			}
			else
			{
				++countDeleteEffect;
				return true;
			}
		});

		if (countDeleteEffect > 0)
		{
			size_t size = _effects.size();
			size -= countDeleteEffect;
			_effects.resize(size);
		}
	}

	// Grider
	{
		size_t countDelete = 0;
		std::remove_if(_gliders.begin(), _gliders.end(), [&countDelete](Glider* item)
		{
			if (item->getLive())
			{
				return false;
			}
			else
			{
				++countDelete;
				return true;
			}
		});

		if (countDelete > 0)
		{
			size_t size = _gliders.size();
			size -= countDelete;
			_gliders.resize(size);
		}
	}

	for (auto object : _objects) object->action();
	for (auto glider : _gliders) glider->action();
	for (auto effect : _effects) effect->action();

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

EffectObject& Map::addEffect(const string& nameModel, const glm::vec3& pos)
{
	EffectObject& effect = help::add(_effects);
	effect.set("", nameModel, PhysicType::NONE, pos);
	return effect;
}

Object& Map::getObjectByName(const string& name)
{
	return help::find(_objects, name);
}

Glider& Map::addGlider(const string& nameTemplate, AIInterface* ai, const glm::vec3& pos, const string& name)
{
	Glider &glider = help::add(_gliders, new Glider(nameTemplate, pos, name));

	if (ai)
		glider.setAi(ai);

	return glider;
}

Glider& Map::getGliderByName(const string& name)
{
	return help::find(_gliders, name);
}


// ststic 

MapPtr Map::_current;

void Map::setCurrent(MapPtr map)
{
	_current = map;
}

