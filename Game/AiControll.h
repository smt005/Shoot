#pragma once

#include "../Engine/Object/Glider.h"
#include "../Engine/Common/IncludesMatem.h"

class AiControll : public AIInterface
{
public:
	inline void setCommand(GliderCommand command)
	{
		if (!_glider)
			return;

		_glider->getCommands()[command] = true;
	}

	inline void setVector(const glm::vec3& vector)
	{
		_vector = vector;
		_glider->setNeedVector(_vector);
	}

private:
	glm::vec3 _vector = glm::vec3(1.0f, 0.0f, 0.0f);
};