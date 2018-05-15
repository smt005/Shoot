#pragma once
//#include <Windows.h>
//#include <string>
using namespace std;

#include "../Common/IncludesMatem.h"
using namespace glm;

struct PhysicPlane
{
private:
public:
	vec3 _p1, _p2, _p3;
	float a, b, c, d;
	vec3 _normal;
	float bottomValue;
	float _area;

	const float _error = 0.001f;

public:
	inline void set(const vec3& p1, const vec3& p2, const vec3& p3)
	{
		_p1 = p1;
		_p2 = p2;
		_p3 = p3;

		set(glm::value_ptr(p1), glm::value_ptr(p2), glm::value_ptr(p3));
	}

	void set(const float* p1, const float* p2, const float* p3)
	{
		_p1 = vec3(p1[0], p1[1], p1[2]);
		_p2 = vec3(p2[0], p2[1], p2[2]);
		_p3 = vec3(p3[0], p3[1], p3[2]);

		float x1 = p1[0];
		float y1 = p1[1];
		float z1 = p1[2];

		float x2 = p2[0];
		float y2 = p2[1];
		float z2 = p2[2];

		float x3 = p3[0];
		float y3 = p3[1];
		float z3 = p3[2];

		a = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
		b = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
		c = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);

		d = x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1);
		d = -d;

		bottomValue = pow(a, 2) + pow(b, 2) + pow(c, 2);
		bottomValue = sqrt(bottomValue);

		if (bottomValue == 0.0f)
			bottomValue = 0.000001f;

		_normal = normalize(vec3(a, b, c));

		areaTriangle();
	}

	inline float dist(const float* pos)
	{
		return (a * pos[0] + b * pos[1] + c * pos[2]) / bottomValue;
	}

	inline float distByVector(const vec3& vector, const vec3& pos = vec3(0.0f))
	{
		return distByVector(value_ptr(vector), value_ptr(pos));
	}

	inline float distByVector(const float* vector, const float* pos)
	{
		return -(a * pos[0] + b * pos[1] + c * pos[2] + d) / (a * vector[0] + b * vector[1] + c * vector[2]);
	}

	vec3 crossVector(const vec3& vector, const vec3& pos = vec3(0.0f))
	{
		float dist = distByVector(vector, pos);

		return vec3 (pos.x + vector.x * dist,
					 pos.y + vector.y * dist,
					 pos.z + vector.z * dist);
	}

	inline float distPointToPlane(const vec3& pos)
	{
		float dist = (a * pos.x + b * pos.y + c * pos.z + d) / bottomValue;
		return dist;
	}

	float entryPointToPlane(const vec3& pos, const float dist, const float error = 0.0f)
	{
		float areaTriangles = 0.0f;

		vec3 posOnPlane(pos);
		vec3 vec(_normal * dist);
		posOnPlane -= vec;

		areaTriangles += areaTriangle(_p1, _p2, posOnPlane);
		areaTriangles += areaTriangle(_p2, _p3, posOnPlane);
		areaTriangles += areaTriangle(_p3, _p1, posOnPlane);

		return areaTriangles <= (_area + error) ? true : false;
	}

	bool entryPointToPlane(const vec3& pos)
	{
		float dist = distPointToPlane(pos);
		
		if (dist > 0)
		{
			//_CrtDbgReport(0, NULL, 0, NULL, "LOG: dist: %f = %s\n", dist, "NO");
			return false;
		}

		vec3 posOnPlane(pos);
		vec3 vec(_normal * dist);
		posOnPlane -= vec;

		float areaTriangles = 0.0f;
		areaTriangles += areaTriangle(_p1, _p2, posOnPlane);
		areaTriangles += areaTriangle(_p2, _p3, posOnPlane);
		areaTriangles += areaTriangle(_p3, _p1, posOnPlane);

		bool res = areaTriangles <= _area ? true : false;
		//_CrtDbgReport(0, NULL, 0, NULL, "LOG: dist: %f - %fm %f = %s\n", dist, areaTriangles,  _area, (res ? "yes" : "NO"));
		return res;
//		return areaTriangles <= _area ? true : false;
	}

	inline void areaTriangle()
	{
		_area = areaTriangle(_p1, _p2, _p3);
		_area += _error;
	}

	inline float areaTriangle(const vec3& p1, const vec3& p2, const vec3& p3)
	{
		float AB = length(p1 - p2);
		float BC = length(p2 - p3);
		float CA = length(p3 - p1);

		float p = (AB + BC + CA) / 2.0f;
		float area = sqrt(p*(p - AB) * (p - BC) * (p - CA));
		
		return area;
	}

	vec3 reflect(const vec3& vectorTarget, const float elasticity = 1.0f)
	{
		if (length(vectorTarget) == 0.0f)
			return vec3(0.0f);

		float speed = length(vectorTarget);
		vec3 vector = normalize(vectorTarget);

		float dot = glm::dot(_normal, vector);
		vec3 valueVector = _normal;
		valueVector *= dot;
		valueVector *= (-2.0f);
		vec3 v(vector + valueVector);
		v *= elasticity;
		v *= speed;

		return v;
	}

	//help

	inline static float lenght(const float* point)
	{
		return sqrt(pow(point[0], 2) + pow(point[1], 2) + pow(point[2], 2));
	}

	inline static float lenght(const glm::vec3& point)
	{
		return sqrt(pow(point[0], 2) + pow(point[1], 2) + pow(point[2], 2));
	}

	inline static float maxLenght(const float* point0, const float* point1, const float* point2)
	{
		float l0 = lenght(point0);
		float l1 = lenght(point1);
		float l2 = lenght(point2);
		float l = 0.0f;

		l0 = l0 > l1 ? l0 : l1;
		l0 = l2 > l0 ? l2 : l0;

		return l0;
	}

	inline static float dist(const glm::vec3& pos0, const glm::vec3& pos1)
	{
		glm::vec3 vec(pos1 - pos0);
		return glm::length(vec);
	}
};