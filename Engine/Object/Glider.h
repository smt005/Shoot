#pragma once

#include "Object.h"
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
	virtual ~AIInterface() { }
	virtual void action() { }

	inline void setGlider(Glider* glider) { _glider = glider; }
	inline Glider* getGlider() { return _glider; }

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
	Glider(const string& nameTemplate);
	virtual ~Glider();
	void setTemplate(const string& nameTemplate);

	inline void setLookVector(const vec3& lookVector) { _lookVector = lookVector; }
	inline void setMoveVector(const vec3& moveVector) { _moveVector = moveVector; }
	inline bool* getCommands() { return _commands; }
	void resetCommand();

	inline int getId() { return reinterpret_cast<int>(this); }
	inline bool getLive() { return _live; }
	inline void setLive(const bool live) { _live = live; }

	void setTemplate(GliderTemplatePtr gliderTemplate);
	void setAi(AIInterface* ai);

	void action();
	void rotate();
	void move();
	void height();
	void shoot();

public:
	static Glider& defaultItem() { return _defaultGlider; }

private:
	static Glider _defaultGlider;
};
