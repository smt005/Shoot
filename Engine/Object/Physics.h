#pragma once

#include "Shape.h"

#include "../Common/IncludesMatem.h"
using namespace glm;

#include "memory"
#include <vector>
using namespace std;

enum class PhysicType
{
	NONE,
	CONVEX,
	TERRAIN
};

class PhysicObject;
typedef weak_ptr<PhysicObject> PhysicObjectWptr;
typedef shared_ptr<PhysicObject> PhysicObjectPtr;

class PhysicObject final
{
public:
	PhysicObject();
	~PhysicObject();

	ShapePtr getShape();
	mat4x4 getWorldTransform();

	static PhysicObjectWptr create(ShapePtr& shape, PhysicType type, const mat4x4 mat);
	static void clear();
	static void remove(const ShapePtr& shape);

private:
	ShapePtr _shape;
	PhysicType _type = PhysicType::NONE;
	mat4x4 _mat = mat4x4(1.0f);

	static vector<PhysicObjectPtr> _objects;
};
