#pragma once

//#include <math.h>

#define _USE_MATH_DEFINES
#include <cmath>

// GLM
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace math
{
	inline float angleCosZ(const glm::vec3& vec0, const glm::vec3& vec1)
	{
		float am = glm::length(vec0) * glm::length(vec1);
		if (am == 0.0f)
			return 0.0f;

		am = abs(am);

		float angle = (vec0.x * vec1.x + vec0.y * vec1.y) / am;

		return angle;
	}

	inline float angleFactorZ(const glm::vec3& vec0, const glm::vec3& vec1)
	{
		return (angleCosZ(vec0, vec1) - 1.0f) / (- 2.0f);
	}
}