#include "Gun.h"
#include "Shell.h"
#include "../App/App.h"
#include "../Common/Log.h"

Gun::Gun(const int gliderId, const string& nameModel)
{
	_gliderId = gliderId;
	_nameModel = nameModel;
}

void Gun::shoot( const glm::vec3& pos, const glm::vec3& vector)
{
	if (!_model)
		_model = Model::getByName(_nameModel);

	double curentTime = App::getCurentTime();
	double time = curentTime - _time;

	if (time > 100)
	{
		Shell::add(pos, vector, _gliderId, _model);
		_time = curentTime;
	}
}