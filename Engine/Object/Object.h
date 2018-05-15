#pragma once

#include "../Physics/Physics.h"
#include "../Common/IncludesMatem.h"
#include "../Common/json.h"
#include "../Callback/Callback.h"
#include "Model.h"

#include <string>
using namespace std;

using json = nlohmann::json;
using namespace glm;

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

	void setSpeed(const vec3& speedVector);
	void setSpeed(const float speed);

	void setPos(const vec3& pos);
	inline void setMatrix(const mat4x4 &matrix) { _matrix = matrix; }
	void setMatrix(const float *matrix);

	void setModel(const string& nameModel);
	void setModel(ModelPtr& model);

	inline ModelPtr& getModelPtr() { return _model; }
	inline string getModelName() { return _model ? _model->name() : ""; }
	Model& getModel();
	const float& getHeight();

	void set(const string &name, const string &modelName, PhysicType physicType, const vec3 &pos = vec3(0.0f), const json &data = json::basic_json());
	void setHeight(const float &height);
	void setVector(const glm::vec3 &vector);
	void setPhysic();

	inline PhysicObjectWptr& getPhysic() { return _physic; }

	// Virtual

	virtual void setData(const json &data);
	virtual void action();

public:
	static Object* _default;

public:
	static Object& defaultItem();
	static void removeDefault();
};
