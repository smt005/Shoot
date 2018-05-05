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
	friend Glider;

public:
	virtual ~AIInterface() { }
	virtual void action() { }

private:
	void setGlider(Glider* glider)
	{
		_glider = glider;
	}

protected:
	Glider* _glider;
};

//---

struct GliderTemplate
{
	const static GliderTemplate _template;

	float _minHeight = 0.9f;
	float _maxHeight = 1.0f;
	float _speedHeight = 0.01f;
	float _speed = 0.05f;
	float _speedRotate = 0.1f;
};

//---

#define GLIDER_COUNT_COMMAND 8
enum GliderCommand
{
	FOWARD = 0,
	BACK = 1,
	LEFT = 2,
	RIGHT = 3,
	JUMP = 4,
	DUCK = 5,
	SHOOT = 6,
	ANY = 7
};

class Glider : public Object
{
private:
	GliderTemplatePtr _template;
	AIptr _ai;
	vec3 _needVector = vec3(1.0f, 0.0f, 0.0f);
	bool _commands[GLIDER_COUNT_COMMAND];

	float _speedHeight;

public:
	Glider();
	virtual ~Glider();

	inline void setNeedVector(const vec3& needVector) { _needVector = needVector; }
	inline bool* getCommands() { return _commands; }
	void resetCommand();

	void setTemplate(GliderTemplatePtr gliderTemplate);
	void setAi(AIInterface* ai);

	void action();
	void move(const glm::vec3 &vector);
	void rotate(const glm::vec3 &vector);

	static Glider& defaultItem() { return _defaultGlider; }

private:
	static Glider _defaultGlider;
};
