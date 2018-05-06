
#include "Physics.h"
#include "../Common/Log.h"

#include <algorithm>

vector<PhysicObjectPtr> PhysicObject::_objects;
vec3 PhysicObject::_gravity = vec3(0.0f, 0.0f, -9.8f);

PhysicObject::PhysicObject()
{

}

PhysicObject::~PhysicObject()
{

}

ShapePtr PhysicObject::getShape()
{
	return _shape;
}

mat4x4 PhysicObject::getWorldTransform()
{
	return _mat;
}

void PhysicObject::setMatrix(const mat4x4& mat)
{
	_mat = mat;
}

void PhysicObject::setPos(const vec3& pos)
{
	_mat[0][0] = pos.x;
	_mat[0][1] = pos.y;
	_mat[0][2] = pos.z;
}

void PhysicObject::setVector(const vec3& vec)
{
	// TODO:
	LOG("LOG: PhysicObject::setVector TODO:\n");
}

void PhysicObject::setAngle(const float angle, const vec3& vec)
{
	_mat = glm::rotate(_mat, angle, vec);
}

void PhysicObject::addForce(const vec3& vector)
{
	_forceVector = vector;
}

PhysicObjectWptr PhysicObject::create(ShapePtr& shape, PhysicType type, const mat4x4 mat)
{
	PhysicObject* object = new PhysicObject();
	
	object->_shape = shape;
	object->_type = type;
	object->_mat = mat;

	PhysicObjectPtr objectPtr = PhysicObjectPtr(object);
	_objects.push_back(objectPtr);

	return objectPtr;
}

void PhysicObject::clear()
{
	_objects.clear();
}

void PhysicObject::remove(const ShapePtr& shape)
{
	vector<PhysicObjectPtr>::iterator it = find_if(_objects.begin(), _objects.end(), [shape](PhysicObjectPtr& object)
	{
		if (!object->_shape)
			return false;

		if (object->_shape->name() == shape->name())
			return true;

		return false;
	});

	if (it == _objects.end())
		return;

	_objects.erase(it);
}

void PhysicObject::update()
{
	for (vector<PhysicObjectPtr>::iterator it = _objects.begin(); it != _objects.end(); ++it)
	{
		PhysicObject* object = (*it).get();

		vec3& _forceVector = object->_forceVector;

		vec3 endForce(_gravity + _forceVector);

		object->applyVector(endForce);

		_forceVector = vec3(0.0f);
	}
}

//---

void PhysicObject::applyVector(const vec3& vector)
{
	_speedVector += vector;

	//LOGI("LOG: _speedVector.z: %f\n", _speedVector.z);

	_mat[3][0] += _speedVector.x;
	_mat[3][1] += _speedVector.y;
	_mat[3][2] += _speedVector.z;
}
