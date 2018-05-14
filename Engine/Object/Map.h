#pragma once

#include "../Physics/Physics.h"
#include "../Common/DataClass.h"
#include "../Common/IncludesMatem.h"

#include "../Common/json.h"
using json = nlohmann::json;

#include <memory>
#include <vector>

class AIInterface;
class Object;
class Glider;
class Map;
class EffectObject;

typedef std::shared_ptr<Map> MapPtr;

class Map : public DataClass <Map>
{
private:
	float _area = 10.0f;

public:
	std::vector<Object*> _objects;
	std::vector<Glider*> _gliders;
	std::vector<EffectObject*> _effects;

	virtual ~Map();

	const float& getArea() { return _area; };

	bool create(const string &newName);
	void setPhysic();

	inline std::vector<Object*>& getObjects()		{ return _objects; }
	inline std::vector<Glider*>& getGliders()		{ return _gliders; }
	inline std::vector<EffectObject*>& getEffects()	{ return _effects; }

	void getDataJson(json& dataJson);
	void action();

	Object& addObjectToPos(const string& nameModel, const PhysicType& type = PhysicType::NONE, const glm::vec3& pos = glm::vec3(0.0, 0.0, 0.0));
	Object& addObject(const string& nameModel, const PhysicType& type = PhysicType::NONE, const glm::mat4x4& mat = glm::mat4x4(1.0));
	EffectObject& addEffect(const string& nameModel, const glm::vec3& pos = glm::vec3(0.0, 0.0, 0.0));

	Object& getObjectByName(const string& name);

	Glider& addGlider(const string& nameTemplate = string("base"), AIInterface* ai = nullptr, const glm::vec3& pos = vec3(0.0f), const string& name = string());
	Glider& getGliderByName(const string& name);

private:
	static MapPtr _current;

public:
	static void setCurrent(MapPtr map);
	inline static MapPtr currentPtr()
	{
		if (!_current)
			_current = MapPtr(new Map());

		return _current;
	}

	inline static Map& current()
	{
		return *currentPtr();
	}
};
