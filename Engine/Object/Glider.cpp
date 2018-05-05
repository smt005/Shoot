
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

	height();
	
	if (countVectors > 0)
	{
		if (length(moveVector) > 0.0f)
		{
			moveVector /= countVectors;
			moveVector = normalize(moveVector);

			move(moveVector);
		}
	}
	
	rotate(_needVector);

	if (_commands[GliderCommand::SHOOT])
		shoot();

	resetCommand();
}

void Glider::move(const glm::vec3 &vector)
{
	glm::vec3 offsetVector = vector * _template->_speed;
	_matrix[3][0] += offsetVector.x;
	_matrix[3][1] += offsetVector.y;
}

void Glider::rotate(const glm::vec3 &vector)
{
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
	_matrix = glm::rotate(_matrix, angle, vectorMatrix);
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
