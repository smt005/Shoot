#pragma once

//#include <math.h>
//#define _USE_MATH_DEFINES
//#include <cmath>

#include "../Common/IncludesMatem.h"
using namespace glm;

struct PhysicPlane
{
private:
	float a, b, c, d;
	float bottomValue;

public:
	inline void set(const vec3& p1, const vec3& p2, const vec3& p3)
	{
		set(glm::value_ptr(p1), glm::value_ptr(p2), glm::value_ptr(p3));
	}

	void set(const float* p1, const float* p2, const float* p3)
	{
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