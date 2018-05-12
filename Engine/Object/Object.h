#pragma once

#include "../Physics/Physics.h"
#include "../Common/IncludesMatem.h"
#include "../Common/json.h"
#include "../Callback/Callback.h"

#include <string>
using namespace std;

using json = nlohmann::json;
using namespace glm;

class Model;
typedef std::shared_ptr<Model> ModelPtr;

class Object;
typedef std::shared_ptr<Object> ObjectPtr;

class Object: public CallbackObject
{
protected:
	string _name;
	mat4x4 _matrix = mat4x4(1.0f);
	ModelPtr _model;
	
	PhysicType _physicType = PhysicType::NONE;
	PhysicObjectWptr _physic;

public:
	Object() {};
	Object(const string &name, const string &modelName, PhysicType physicType = PhysicType::NONE, const vec3 &pos = vec3(0.0f), const json &data = json::basic_json());
	virtual ~Object();

	void setName(const string& name) { _name = name; };

	const string& name() { return _name; };
	const mat4x4& getMatrix() { return _matrix; };
	const float* matrixFloat() { return value_ptr(_matrix); };
	vec3 getPos();
	vec3 getVector();
	void getDataJson(json& dataJson);

	inline void addPos(const vec3& offset) { _matrix[3][0] = _matrix[3][0] + offset.x; _matrix[3][1] = _matrix[3][1] + offset.y; _matrix[3][2] = _matrix[3][2] + offset.z; }

	void setPos(const vec3& pos);
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
