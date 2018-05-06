#pragma once

#include "Shape.h"

#include "../Common/IncludesMatem.h"
using namespace glm;

#include <memory>
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

	inline const vec3& getSpeed() { return _speedVector; }

	void setMatrix(const mat4x4& mat);
	void setPos(const vec3& pos);
	void setVector(const vec3& vec);
	void setAngle(const float angle, const vec3& vec);

	void addForce(const vec3& vector);

	static PhysicObjectWptr create(ShapePtr& shape, PhysicType type, const mat4x4 mat);
	static void clear();
	static void remove(const ShapePtr& shape);
	static void update();

	inline static void setGravity(const vec3& gravity) { _gravity = gravity; };
	inline static const vec3& getGravity() { return _gravity; }

private:
	void applyVector(const vec3& vector);

private:
	ShapePtr _shape;
	PhysicType _type = PhysicType::NONE;
	
	mat4x4 _mat = mat4x4(1.0f);
	float _mass = 1.0f;

	vec3 _speedVector = vec3(0.0f);

	vec3 _forceVector = vec3(0.0f);

private:
	static vector<PhysicObjectPtr> _objects;
	static vec3 _gravity;
};
