#pragma once

#include "Model.h"

#include "../Common/IncludesMatem.h"

#include <string>
#include <memory>
using namespace std;

class Gun;
typedef shared_ptr<Gun> GunPtr;

class Gun final
{
public:
	Gun(const int gliderId, const string& nameModel = "BoxShell");

	void shoot(const glm::vec3& pos, const glm::vec3& vector);

private:
	string _nameModel;
	ModelPtr _model;
	int _gliderId = 0;
	double _time;
};