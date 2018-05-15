#pragma once

#include "PhysicObject.h"

#include "../Common/IncludesMatem.h"
using namespace glm;

#include <vector>
using namespace std;

class Physics
{
public:
	static PhysicObjectWptr create(ShapePtr& shape, PhysicType type, const mat4x4 mat);
	static void clear();
	static void remove(const ShapePtr& shape);
	static void update();

	static bool collision(PhysicObject* object);
	static bool hint(PhysicObject* object, PhysicObject* target, vec3& vector);

	inline static void setGravity(const vec3& gravity) { _gravity = gravity; };
	inline static const vec3& getGravity() { return _gravity; }

private:
	static vector<PhysicObjectPtr> _objects;
	static vec3 _gravity;
};
