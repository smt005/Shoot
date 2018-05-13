#include "Shell.h"
#include "Model.h"
#include "Glider.h"
#include "Map.h"
#include "../App/App.h"

using namespace glm;

deque<Shell*> Shell::_shells;

Shell::Shell(const vec3& pos, const vec3& vector, const int gliderId, ModelPtr& model):
	_time(App::getCurentTime()),
	_gliderId(gliderId)
{
	setPos(pos);
	_vector = vector;
	_vector.z = 0.0f;

	_model = model;
}

bool Shell::hasHint()
{
	// TODO: Доступ к карте нужен быстрее

	MapPtr mapPtr = Map::getByName("MapGameTerrain");
	vec3 pos = getPos();

	for (Glider* glider : mapPtr->getGliders())
	{
		if (glider->getId() == _gliderId)
			continue;

		if (!glider->getLive())
			continue;

		vec3 vector = glider->getPos() - pos;
		float dist = glm::length(vector);

		if (dist < 5.0f)
		{
			_time = 0;
			glider->setLive(false);
			return true;
		}
	}

	return false;
}

void Shell::add(const glm::vec3& pos, const glm::vec3& vector, const int gliderId, ModelPtr& model)
{
	Shell* shell = new Shell(pos, vector, gliderId, model);
	_shells.emplace_back(shell);
}

void Shell::update()
{
	double currentTime = App::getCurentTime();

	for (deque<Shell*>::iterator it = _shells.begin(); it != _shells.end(); ++it)
	{
		double pastTime = currentTime - (*it)->_time;

		if (pastTime > 2000)
		{
			delete (*it);
			_shells.erase(it);
			break;
		}
	}

	for (deque<Shell*>::iterator it = _shells.begin(); it != _shells.end(); ++it)
	{
		Shell* shell = *it;

		if (shell && !shell->hasHint())
			shell->addPos(shell->_vector);
	}
}
