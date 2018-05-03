#pragma once

#include "Object.h"
#include "../Common/IncludesMatem.h"

#include "memory"
using namespace std;

class Glider;
typedef shared_ptr<Glider> GliderPtr;
typedef weak_ptr<Glider> GliderWptr;

class AIInterface;
typedef shared_ptr<AIInterface> AIptr;

struct GliderTemplate;
typedef shared_ptr<GliderTemplate> GliderTemplatePtr;

class AIInterface
{
public:
	AIInterface(GliderWptr glider)
	{
		_glider = glider;
	}

	virtual ~AIInterface() { }
	virtual void action() { }

private:
	GliderWptr _glider;
};

struct GliderTemplate
{
	const static GliderTemplate _template;

	float _minHeight = 0.9f;
	float _maxHeight = 1.0f;
	float _speedHeight = 0.01f;
	float _speed = 0.05f;
};

class Glider : public Object
{
private:
	GliderPtr _self;
	GliderTemplatePtr _template;
	AIptr _ai;

	float _speedHeight;

public:
	Glider();
	virtual ~Glider();

	void action();

	void move(const glm::vec3 &vector);
};
