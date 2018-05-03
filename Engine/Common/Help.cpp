
#include "Help.h"

#include <stdlib.h>
//#include <functional>

using namespace glm;

bool help::intersection(vec3 start1, vec3 end1, vec3 start2, vec3 end2, vec3* out_intersection)
{
	vec3 dir1 = end1 - start1;
	vec3 dir2 = end2 - start2;

	//считаем уравнения прямых проходящих через отрезки
	float a1 = -dir1.y;
	float b1 = +dir1.x;
	float d1 = -(a1*start1.x + b1 * start1.y);

	float a2 = -dir2.y;
	float b2 = +dir2.x;
	float d2 = -(a2*start2.x + b2 * start2.y);

	//подставляем концы отрезков, для выяснения в каких полуплоскотях они
	float seg1_line2_start = a2 * start1.x + b2 * start1.y + d2;
	float seg1_line2_end = a2 * end1.x + b2 * end1.y + d2;

	float seg2_line1_start = a1 * start2.x + b1 * start2.y + d1;
	float seg2_line1_end = a1 * end2.x + b1 * end2.y + d1;

	//если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет.
	if (seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0)
		return false;

	float u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);
	*out_intersection = start1 + u * dir1;

	return true;
}

/*
	v1 = rand() % 100;         // v1 in the range 0 to 99
	v2 = rand() % 100 + 1;     // v2 in the range 1 to 100
	v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014
*/

float help::random_f(const float& min, const float& max, const int& accuracy)
{
	int var = rand() % accuracy;
	float k = static_cast<float>(var) / static_cast<float>(accuracy);

	float range = max- min;
	float value = min + range * k;

	return value;
}

int help::random_i(const int& min, const int& max)
{
	const int range = max - min;
	int var = rand() % range;
	float k = static_cast<float>(var) / static_cast<float>(range);

	float value = min + range * k;

	return static_cast<int>(value);
}
