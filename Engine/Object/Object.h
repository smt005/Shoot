#pragma once

#include "Physics.h"
#include "../Common/IncludesMatem.h"
#include "../Common/json.h"

#include <string>
using namespace std;

using json = nlohmann::json;
using namespace glm;

class Model;
typedef std::shared_ptr<Model> ModelPtr;

enum ObjectType
{
	PHYSIC_DEFAULT = 0,
	PHYSIC_GENERATOR,
	PHYSIC_CAR
};

class Object
{
//private:
protected:
	ObjectType _type = ObjectType::PHYSIC_DEFAULT;
	string _name;
	mat4x4 _matrix = mat4x4(1.0f);
	ModelPtr _model;

	PhysicType _physicType = PhysicType::NONE;
	PhysicObjectWptr _physic;

public:
	Object();
	Object(const string &name, const string &modelName, PhysicType physicType = PhysicType::NONE, const vec3 &pos = vec3(0.0f), const json &data = json::basic_json());
	virtual ~Object();

	void setName(const string& name) { _name = name; };

	const ObjectType& type() { return _type; };
	const string& name() { return _name; };
	const mat4x4& getMatrix() { return _matrix; };
	const float* matrixFloat() { return value_ptr(_matrix); };
	void getDataJson(json& dataJson);

	void setMatrix(const mat4x4 &matrix) { _matrix = matrix; };
	void setMatrix(const float *matrix);

	Model& getModel();
	const float& getHeight();

	void set(const string &name, const string &modelName, PhysicType physicType, const vec3 &pos = vec3(0.0f), const json &data = json::basic_json());
	void setHeight(const float &height);
	void setVector(const glm::vec3 &vector);
	void setPhysic();

	// Virtual

	virtual void setData(const json &data);
	virtual void action();

public:
	static Object* _default;

public:
	static Object& defaultItem();
	static void removeDefault();
};
