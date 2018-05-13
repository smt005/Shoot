
// TODO: Временный класс для снаряда

#pragma once

#include "Object.h"
#include "Model.h"
#include "../Common/IncludesMatem.h"

#include <deque>
using namespace std;

class Shell final : public Object
{
public:
	Shell(const glm::vec3& pos, const glm::vec3& vector, const int gliderId, ModelPtr& model);

	bool hasHint();

private:
	glm::vec3 _vector;
	double _time = 0;
	int _gliderId = 0;

public:
	static void add(const glm::vec3& pos, const glm::vec3& vector, const int gliderId, ModelPtr& model);
	static void update();
	
	static inline deque<Shell*>& getShells() { return _shells; }
	
private:
	static deque<Shell*> _shells;
};