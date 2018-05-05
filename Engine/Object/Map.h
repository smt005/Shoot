#pragma once

#include "Physics.h"
#include "../Common/DataClass.h"
#include "../Common/IncludesMatem.h"

#include "../Common/json.h"
using json = nlohmann::json;

#include <memory>
#include <vector>

class Object;
class Glider;
class Map;

//typedef std::shared_ptr<Glider> GliderPtr;
typedef std::shared_ptr<Map> MapPtr;

class Map : public DataClass <Map>
{
private:
	float _area = 10.0f;

public:
	std::vector<Object*> _objects;
	std::vector<Glider*> _gliders;

	Map();
	virtual ~Map();

	const float& getArea() { return _area; };

	bool create(const string &newName);
	void setPhysic();

	void getDataJson(json& dataJson);
	void action();

	Object& addObjectToPos(const string& nameModel, const PhysicType& type = PhysicType::NONE, const glm::vec3& pos = glm::vec3(0.0, 0.0, 0.0));
	Object& addObject(const string& nameModel, const PhysicType& type = PhysicType::NONE, const glm::mat4x4& mat = glm::mat4x4(1.0));

	Glider& getGliderByName(const string& name);
};
