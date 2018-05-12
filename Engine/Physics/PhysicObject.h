#pragma once

#include "PhysicPlane.h"
#include "../Object/Shape.h"

#include "../Common/IncludesMatem.h"
using namespace glm;

#include <memory>
#include <vector>
using namespace std;

class Physics;

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
	friend Physics;

public:
	PhysicObject();
	~PhysicObject();

	ShapePtr getShape();
	mat4x4 getWorldTransform();

	inline const vec3& getSpeed() { return _speedVector; }
	inline float getSpeedf() { return glm::length(_speedVector); }
	inline const PhysicPlane* getPlanes() { return _planes; }
	inline vec3 getPos() { return vec3(_mat[3][0], _mat[3][1], _mat[3][2]); }
	
	void setMatrix(const mat4x4& mat);
	void setPos(const vec3& pos);
	void setVector(const vec3& vec);
	void setAngle(const float angle, const vec3& vec);

	void addForce(const vec3& vector);

private:
	void applyVector(const vec3& vector);

	void calculating();

private:
	ShapePtr _shape;
	PhysicType _type = PhysicType::NONE;
	float _radius = 0.0f;

	unsigned int _countPlane = 0;
	PhysicPlane* _planes = nullptr;

	mat4x4 _mat = mat4x4(1.0f);
	float _mass = 1.0f;

	vec3 _speedVector = vec3(0.0f);
	vec3 _forceVector = vec3(0.0f);
};
