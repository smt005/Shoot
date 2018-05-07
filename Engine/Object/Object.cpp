
#include "Object.h"
#include "Model.h"
#include "Shape.h"
#include "../Physics/Physics.h"

Object::Object()
{

}

Object::Object(const string &name, const string &modelName, PhysicType physicType, const vec3 &pos, const json &data)
{
	set(name, modelName, physicType, pos, data);
}

Object::~Object()
{
}

vec3 Object::getPos()
{
	return vec3(_matrix[3][0], _matrix[3][1], _matrix[3][2]);
}

vec3 Object::getVector()
{
	vec4 vector(1.0f, 0.0f, 0.0f, 0.0f);
	vector = _matrix * vector;

	vec3 resVector(vector.x, vector.y, vector.z);
	return resVector;
}

void Object::getDataJson(json& dataJson)
{
	dataJson["name"] = name();
	dataJson["model"] = getModel().name();

	dataJson["pos"].push_back(_matrix[3][0]);
	dataJson["pos"].push_back(_matrix[3][1]);
	dataJson["pos"].push_back(_matrix[3][2]);
}

void Object::setPos(const vec3& pos)
{
	_matrix[3][0] = pos[0];
	_matrix[3][1] = pos[1];
	_matrix[3][2] = pos[2];
}

void Object::setMatrix(const float *matrix)
{
	_matrix[0][0] = matrix[0];
	_matrix[0][1] = matrix[1];
	_matrix[0][2] = matrix[2];
	_matrix[0][3] = matrix[3];

	_matrix[1][0] = matrix[4];
	_matrix[1][1] = matrix[5];
	_matrix[1][2] = matrix[6];
	_matrix[1][3] = matrix[7];

	_matrix[2][0] = matrix[8];
	_matrix[2][1] = matrix[9];
	_matrix[2][2] = matrix[10];
	_matrix[2][3] = matrix[11];

	_matrix[3][0] = matrix[12];
	_matrix[3][1] = matrix[13];
	_matrix[3][2] = matrix[14];
	_matrix[3][3] = matrix[15];
}

Model& Object::getModel()
{
	if (!_model) _model = Model::getByName("default");
	return *_model;
};

const float& Object::getHeight()
{
	return _matrix[3][2];
}

void Object::set(const string &name, const string &modelName, PhysicType physicType, const vec3 &pos, const json &data)
{
	_name = name;
	_model = Model::getByName(modelName);
	_physicType = physicType;

	if (length(pos) > 0.0f) _matrix = translate(_matrix, pos);
	if (!data.empty()) setData(data);
}

void Object::setHeight(const float &height)
{
	_matrix[3][2] = height;
}

void Object::setVector(const glm::vec3 &vector)
{

}

void Object::setPhysic()
{
	if (_model)
	{
		ShapePtr& shape = _model->getShapePtr();
		_physic = Physics::create(shape, _physicType, _matrix);
	}
}

// Virtual

void Object::setData(const json &data)
{
}

void Object::action()
{
	if (_physic.expired())
		return;

	_matrix = _physic.lock()->getWorldTransform();
}

Object* Object::_default = nullptr;

Object& Object::defaultItem()
{
	if (!_default)
	{
		_default = new Object();
		_default->setName("DEFAULT_OBJECT");
	}

	return *_default;
}

void Object::removeDefault()
{
	delete _default;
	_default = nullptr;
}
