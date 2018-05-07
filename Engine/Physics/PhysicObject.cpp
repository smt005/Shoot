
#include "Physics.h"
#include "../Common/Log.h"

#include <algorithm>

PhysicObject::PhysicObject()
{

}

PhysicObject::~PhysicObject()
{
	if (_planes)
	{
		delete[] _planes;
		_planes = nullptr;
	}
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

//---

void PhysicObject::applyVector(const vec3& vector)
{
	_speedVector += vector;

	//LOGI("LOG: _speedVector.z: %f\n", _speedVector.z);

	_mat[3][0] += _speedVector.x;
	_mat[3][1] += _speedVector.y;
	_mat[3][2] += _speedVector.z;
}

//---

void PhysicObject::calculating()
{
	if (!_shape)
		return;

	unsigned short int _countVertex = _shape->_countVertex;
	float* _aVertex = _shape->_aVertex;

	unsigned short int _countIndex = _shape->_countIndex;
	unsigned short* _aIndex = _shape->_aIndex;

	//LOGI("LOG: [%s]\n", _shape->name().c_str());

	for (int iIndex = 0; iIndex < _countIndex; iIndex += 3)
	{
		int i1 = _aIndex[iIndex];
		int i2 = _aIndex[iIndex + 1];
		int i3 = _aIndex[iIndex + 2];

		/*LOGI("LOG: index [%d", i1);
		LOGI(" %d ", i2);
		LOGI(" %d]\n", i3);*/

		{
			PhysicPlane plane;

			float* p1 = &_aVertex[i1 * 3];
			float* p2 = &_aVertex[i2 * 3];
			float* p3 = &_aVertex[i3 * 3];

			float l = PhysicPlane::maxLenght(p1, p2, p3);
			if (l > _radius)
				_radius = l;

			/*LOGI("LOG: points [%f", p1[0]);
			LOGI(" %f ", p1[1]);
			LOGI(" %f] ", p1[2]);

			LOGI(" [%f", p2[0]);
			LOGI(" %f ", p2[1]);
			LOGI(" %f] ", p2[2]);

			LOGI(" [%f", p3[0]);
			LOGI(" %f ", p3[1]);
			LOGI(" %f]\n", p3[2]);*/

			plane.set(p1, p2, p3);

			/*LOGI("LOG: plane [%f", plane.a);
			LOGI(" %f ", plane.b);
			LOGI(" %f ", plane.c);
			LOGI(" %f]\n", plane.d);*/
		}
	}

	//LOG("LOG:");
}
