#include "AiTarget.h"
#include "../Engine/Object/Map.h"
#include "../Engine/Common/Help.h"
#include "../Engine/Common/IncludesMatem.h"

#include <glm/vec3.hpp>
using namespace glm;

void AiTarget::action()
{
	if (!_glider)
		return;

	bool* commands = _glider->getCommands();

	Glider& target = Map::current().getGliderByName("Glider_player");

	if (!target.getLive())
		return;

	vec3 posTarget = target.getPos();
	posTarget.z = 0.0f;
	vec3 pos = _glider->getPos();
	pos.z = 0.0f;

	vec3 lookVector(posTarget - pos);
	float dist = length(lookVector);

	if (dist < 3.0f)
	{
		commands[GliderCommand::BACK] = true;
	}
	else if (dist > 6.0f)
	{
		commands[GliderCommand::FOWARD] = true;
	}

	lookVector = normalize(lookVector);
	_glider->setLookVector(lookVector);
}
