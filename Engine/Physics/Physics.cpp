
#include "Physics.h"
#include "PhysicPlane.h"
#include "../Common/Log.h"

#include <algorithm>

vector<PhysicObjectPtr> Physics::_objects;
vec3 Physics::_gravity = vec3(0.0f, 0.0f, -9.8f);

PhysicObjectWptr Physics::create(ShapePtr& shape, PhysicType type, const mat4x4 mat)
{
	PhysicObject* object = new PhysicObject();
	
	object->_shape = shape;
	object->_type = type;
	object->_mat = mat;

	object->calculating();

	PhysicObjectPtr objectPtr = PhysicObjectPtr(object);
	_objects.push_back(objectPtr);

	return objectPtr;
}

void Physics::clear()
{
	_objects.clear();
}

void Physics::remove(const ShapePtr& shape)
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

void Physics::update()
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

bool Physics::collision(PhysicObject* object)
{
	for (vector<PhysicObjectPtr>::iterator it = _objects.begin(); it != _objects.end(); ++it)
	{
		PhysicObject* target = (*it).get();

		if (object == target)
			continue;

		float dist = PhysicPlane::dist(object->getPos(), target->getPos());
		dist = dist - (object->_radius + target->_radius);

		//LOGI("LOG: dist = %f", dist);
		//LOGI("\t%s ->", object->_shape->name().c_str());
		//LOGI("\t%s\n", target->_shape->name().c_str());

		if (dist > 0.0f)
			continue;

		hint(object, target);
	}

	return false;
}

bool Physics::hint(PhysicObject* object, PhysicObject* target)
{
	return false;
}
