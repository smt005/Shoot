
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

		if (object->_type == PhysicType::TERRAIN)
			continue;

		if (object->getSpeedf() != 0.0f)
			collision(object);

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

		// TODO: TEMP
		if (target->_type == PhysicType::CONVEX)
			continue;

		/*float dist = PhysicPlane::dist(object->getPos(), target->getPos());
		dist = dist - (object->_radius + target->_radius);

		if (dist > 0.0f)
			continue;*/

		vec3 vector(object->_speedVector);
		bool entry = hint(object, target, vector);

		if (entry)
		{
			object->setSpeed(vector);
			_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: YES: [ %f %f %f ] pos [ %f %f %f ]\n", object->_speedVector.x, object->_speedVector.y, object->_speedVector.z, object->_mat[3][0], object->_mat[3][1], object->_mat[3][2]);
			object->setHasCollision(true);
		}
		else
		{
			_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: no: [ %f %f %f ] pos [ %f %f %f ]\n", object->_speedVector.x, object->_speedVector.y, object->_speedVector.z, object->_mat[3][0], object->_mat[3][1], object->_mat[3][2]);
			object->setHasCollision(false);
		}
	}

	return false;
}

bool Physics::hint(PhysicObject* object, PhysicObject* target, vec3& vector)
{
	int countPlanes = target->getCountPlane();
	PhysicPlane* planes = target->getPlanes();
	vec3 pos = object->getPos();

	if (countPlanes == 0 || !planes)
		return false;

	for (int i = 0; i < countPlanes; ++i)
	{
		if (planes[i].entryPointToPlane(pos))
		{
			vector = planes[i].reflect(vector);
			return true;
		}
	}

	return false;
}
