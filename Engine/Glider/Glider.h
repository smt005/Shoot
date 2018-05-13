#pragma once

#include "../Object/Object.h"
#include "GliderTemplate.h"
#include "Gun.h"
#include "../Common/IncludesMatem.h"

#include "memory"
using namespace std;

class PhysicObject;

class Glider;
typedef shared_ptr<Glider> GliderPtr;
typedef weak_ptr<Glider> GliderWptr;

class AIInterface;
typedef shared_ptr<AIInterface> AIptr;

class AIInterface
{
	friend Glider;

public:
	virtual ~AIInterface()	{ }
	virtual void action()	{ }

	inline void setGlider(Glider* glider)	{ _glider = glider; }
	inline Glider* getGlider()				{ return _glider; }

protected:
	Glider* _glider;
};

//---

enum GliderCommand
{
	FOWARD,
	BACK,
	LEFT,
	RIGHT,

	FOWARD_VIEW,
	BACK_VIEW,
	LEFT_VIEW,
	RIGHT_VIEW,

	JUMP,
	DUCK,
	SHOOT,
	ANY,
	COUNT_COMMANDS	// Количество команд, это значение всегда должно быть в конце.
};

class Glider : public Object
{
private:
	GliderTemplatePtr _template;
	AIptr _ai;
	GunPtr _gunPtr;

	bool _live = true;
	vec3 _lookVector = vec3(1.0f, 0.0f, 0.0f);
	vec3 _moveVector = vec3(1.0f, 0.0f, 0.0f);
	float heightSpeed = 0.0f;
	bool _commands[GliderCommand::COUNT_COMMANDS];

public:
	Glider();
	Glider(const string& nameTemplate, const glm::vec3& pos = vec3(0.0f), const string& name = string());
	virtual ~Glider();

	void setTemplate(const string& nameTemplate);
	void setTemplate(GliderTemplatePtr gliderTemplate);
	void setAi(AIInterface* ai);

	inline void setLookVector(const vec3& lookVector)	{ _lookVector = lookVector; }
	inline void setMoveVector(const vec3& moveVector)	{ _moveVector = moveVector; }
	inline bool* getCommands()							{ return _commands; }
	void resetCommand();

	inline int getId()						{ return reinterpret_cast<int>(this); }
	inline bool getLive()					{ return _live; }
	void setLive(const bool live);

	void action();
	void rotate();
	void move();
	void height();
	void shoot();

public:
	inline static Glider& defaultItem()
	{
		if (!_defaultGlider)
			_defaultGlider = new Glider();

		return *_defaultGlider;
	}

private:
	static Glider* _defaultGlider;
};
