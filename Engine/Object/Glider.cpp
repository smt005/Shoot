
#include "Glider.h"
#include "Shell.h"
#include "../Common/Log.h"

Glider Glider::_defaultGlider;

Glider::Glider()
{
	_template = make_shared<GliderTemplate>();

	_speedHeight = _template->_speedHeight;
	setHeight(_template->_minHeight);

	Gun* gun = new Gun(getId());
	_gunPtr = GunPtr(gun);
}

Glider::~Glider()
{
}

void Glider::resetCommand()
{
	for (bool& item : _commands)
		item = false;
}

void Glider::setTemplate(GliderTemplatePtr gliderTemplate)
{
	_template = gliderTemplate;
}

void Glider::setAi(AIInterface* ai)
{
	if (!ai)
		return;

	ai->setGlider(this);
	_ai = AIptr(ai);
}

void Glider::action()
{
	if (_physic.expired())
		return;

	if (_ai)
		_ai->action();

	float countVectors = 0;
	vec3 moveVector(0.0f);

	_needVector = glm::normalize(_needVector);

	if (_commands[GliderCommand::FOWARD])
	{
		moveVector += _needVector;
		countVectors += 1.0f;
	}

	if (_commands[GliderCommand::BACK])
	{
		moveVector -= _needVector;
		countVectors += 1.0f;
	}

	if (_commands[GliderCommand::LEFT])
	{
		moveVector.x -= _needVector.y;
		moveVector.y += _needVector.x;

		countVectors += 1.0f;
	}

	if (_commands[GliderCommand::RIGHT])
	{
		moveVector.x += _needVector.y;
		moveVector.y -= _needVector.x;

		countVectors += 1.0f;
	}

	//---

	PhysicObjectPtr physic = _physic.lock();
	//height();
	
	if (countVectors > 0)
	{
		if (length(moveVector) > 0.0f)
		{
			moveVector /= countVectors;
		}
	}
	else
	{
		if (!_commands[GliderCommand::FOWARD] && !_commands[GliderCommand::BACK] && !_commands[GliderCommand::LEFT] && !_commands[GliderCommand::RIGHT])
		{
			moveVector = -physic->getSpeed();
		}
	}

	if (glm::length(moveVector) != 0.0f)
	{
		moveVector = normalize(moveVector);
	}
	else
	{
		moveVector.x = 0.0f;
		moveVector.y = 0.0f;
		moveVector.z = 0.0f;
	}

	move(*physic, moveVector);
	rotate(*physic, _needVector);

	if (_commands[GliderCommand::SHOOT])
		shoot();

	resetCommand();
	_matrix = _physic.lock()->getWorldTransform();
}

void Glider::move(PhysicObject& physicObject, const glm::vec3 &vector)
{
	if (glm::length(vector) == 0.0f)
		return;

	glm::vec3 offsetVector = vector * _template->_speed;
	offsetVector.z = 0.0f;

	physicObject.addForce(offsetVector);
}

void Glider::rotate(PhysicObject& physicObject, const glm::vec3 &vector)
{
	// TODO:

	if (length(vector) == 0.0f)
		return;

	vec3 vectorGlider = getVector();

	float am = glm::length(vector) * glm::length(vectorGlider);
	if (am == 0.0f)
		return;

	float ab = vector.x * vectorGlider.x + vector.y * vectorGlider.y;
	float angle = ab / am;

	if (angle == 0.0f)
		return;

	float absAngle = abs(angle);
	if (absAngle < 0.1)
		return;

	float side = angle / absAngle;
	angle = _template->_speedRotate * side;

	vec3 vectorMatrix(0.0f, 0.0f, 1.0f);
	physicObject.setAngle(angle, vectorMatrix);
}

void Glider::height()
{
	float height = getHeight();

	if (height > _template->_maxHeight)
	{
		_speedHeight = -_template->_speedHeight;
	}

	if (height < _template->_minHeight)
	{
		_speedHeight = _template->_speedHeight;
	}

	if (!_live)
		_speedHeight = -_template->_speedHeight * 5.0f;

	setHeight(height + _speedHeight);
}

void Glider::shoot()
{
	if (_gunPtr)
		_gunPtr->shoot(getPos(), _needVector);
}
