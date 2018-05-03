
#include "Physics.h"
#include <algorithm>

vector<PhysicObjectPtr> PhysicObject::_objects;

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
	return mat4x4(1.0f);
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