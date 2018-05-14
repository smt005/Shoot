
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

void PhysicObject::setMatrix(const float* mat)
{
	if (!mat)
		return;

	_mat[0][0] = mat[0];
	_mat[0][1] = mat[1];
	_mat[0][2] = mat[2];
	_mat[0][3] = mat[3];

	_mat[1][0] = mat[4];
	_mat[1][1] = mat[5];
	_mat[1][2] = mat[6];
	_mat[1][3] = mat[7];

	_mat[2][0] = mat[8];
	_mat[2][1] = mat[9];
	_mat[2][2] = mat[10];
	_mat[2][3] = mat[11];

	_mat[3][0] = mat[12];
	_mat[3][1] = mat[13];
	_mat[3][2] = mat[14];
	_mat[3][3] = mat[15];
}

void PhysicObject::setMatrix(const mat4x4& mat)
{
	_mat = mat;
}

void PhysicObject::setPos(const vec3& pos)
{
	_mat[3][0] = pos.x;
	_mat[3][1] = pos.y;
	_mat[3][2] = pos.z;
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

	_countPlane = _countIndex / 3;
	_planes = new PhysicPlane[_countPlane];
	int iPlane = 0;

	for (int iIndex = 0; iIndex < _countIndex; iIndex += 3)
	{
		int i1 = _aIndex[iIndex];
		int i2 = _aIndex[iIndex + 1];
		int i3 = _aIndex[iIndex + 2];

		{
			PhysicPlane plane;

			float* p1 = &_aVertex[i1 * 3];
			float* p2 = &_aVertex[i2 * 3];
			float* p3 = &_aVertex[i3 * 3];

			float l = PhysicPlane::maxLenght(p1, p2, p3);
			if (l > _radius)
				_radius = l;

			_planes[iPlane].set(p1, p2, p3);
			++iPlane;
		}
	}
}
