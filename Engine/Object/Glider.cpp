
#include "Glider.h"
#include "Shell.h"
#include "../Common/Log.h"

Glider* Glider::_defaultGlider = nullptr;

Glider::Glider()
{
	setTemplate("base");
}

Glider::Glider(const string& nameTemplate, const vec3& pos, const string& name)
{
	setTemplate(nameTemplate);
	setName(name);
	setPos(pos);
}

Glider::~Glider()
{
}

void Glider::setTemplate(const string& nameTemplate)
{
	GliderTemplatePtr gliderTemplate = GliderTemplate::getByName(nameTemplate);
	setTemplate(gliderTemplate);
}

void Glider::setTemplate(GliderTemplatePtr gliderTemplate)
{
	if (!gliderTemplate)
		return;

	_template = gliderTemplate;

	_model = Model::getByName(_template->model);

	Gun* gun = new Gun(getId());
	_gunPtr = GunPtr(gun);
}

void Glider::resetCommand()
{
	for (bool& item : _commands)
		item = false;
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
	height();

	if (!_ai)
		return;

	// Rotate
	rotate();

	_moveVector = vec3(0.0f);
	vec3 vector(1.0f, 0.0f, 0.0);

	// Move
	if (_commands[GliderCommand::FOWARD])
	{
		_moveVector += vector;
	}

	if (_commands[GliderCommand::BACK])
	{
		_moveVector -= vector;
	}

	if (_commands[GliderCommand::LEFT])
	{
		_moveVector.x -= vector.y;
		_moveVector.y += vector.x;
	}

	if (_commands[GliderCommand::RIGHT])
	{
		_moveVector.x += vector.y;
		_moveVector.y -= vector.x;
	}

	if (_moveVector.x != 0.0f || _moveVector.y != 0.0f)
	{
		normalize(_moveVector);
		move();
	}

	// Shoot
	if (_commands[GliderCommand::SHOOT])
		shoot();

	resetCommand();
}

void Glider::rotate()
{
	vec3 vectorGlider = getVector();
	float angle = math::angleCosZ(vectorGlider, _lookVector);

	if (angle != 0.0f)
	{
		vec3 vectorMatrix(0.0f, 0.0f, 1.0f);
		_matrix = glm::rotate(_matrix, angle, vectorMatrix);
	}
}

void Glider::move()
{
	glm::vec3 offsetVector = _moveVector * _template->speed;

	_matrix[3][0] += offsetVector.x;
	_matrix[3][1] += offsetVector.y;
	_matrix[3][2] += offsetVector.z;
}

void Glider::height()
{
	if (!_template)
		return;

	float height = getHeight();

	if (height >= _template->maxHeight)
		heightSpeed = -_template->speedHeight;
	else if (height <= _template->minHeight)
		heightSpeed = _template->speedHeight;

	height += heightSpeed;
	setHeight(height);
}

void Glider::shoot()
{
	if (_gunPtr)
		_gunPtr->shoot(getPos(), _lookVector);
}
