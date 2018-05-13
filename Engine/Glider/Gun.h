#pragma once

#include "../Object/Model.h"
#include <glm/vec3.hpp>

#include <string>
#include <memory>
using namespace std;

class Gun;
typedef shared_ptr<Gun> GunPtr;

class Gun final
{
public:
	Gun() {};
	Gun(const int gliderId, const string& nameModel = "BoxShell");

	void shoot(const glm::vec3& pos, const glm::vec3& vector);

private:
	string _nameModel;
	ModelPtr _model;
	int _gliderId = 0;
	double _time = 0.0;
};