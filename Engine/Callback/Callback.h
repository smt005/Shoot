#pragma once

#include "VirtualKey.h"
#include "../Common/IncludesMatem.h"

#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>
using namespace std;

#include "../Common/json.h"
using json = nlohmann::json;

#define CALLBACK_COUNT_KEY 1024

class CallbackHandler;
class CallbackObject;
class Callback;

typedef shared_ptr<CallbackHandler> CallbackHandlerPtr;

typedef std::function<bool(void *data)> CallbackFunction;
#define Function(function) [this](void *data) { return this->function(data); }

enum class EventCallback
{
	TAP_DOWN,
	TAP_PINCH,
	TAP_UP,
	TAP_LONG,
	TAP_DOUBLE,
	MOVE,
	HINT,

	BUTTON_DOWN,
	BUTTON_PINCH,
	BUTTON_UP
};

class CallbackHandler final
{
	friend CallbackObject;
	friend Callback;

public:
	inline int getId()
	{
		CallbackHandler* callbackHandler = this;
		return reinterpret_cast<int>(callbackHandler);
	}

	void set(const EventCallback event, const CallbackFunction &function);
	void runFunction(const EventCallback event);

public:
	json data;

private:
	bool _tap;
	double _time;
	bool _wasLongTap = false;

	CallbackFunction _tap_down = 0;
	CallbackFunction _tap_pinch = 0;
	CallbackFunction _tap_up = 0;
	CallbackFunction _tap_long = 0;
	CallbackFunction _tap_double = 0;
	CallbackFunction _move = 0;
	CallbackFunction _hint = 0;

	CallbackFunction _buttonDown = 0;
	CallbackFunction _buttonPinch = 0;
	CallbackFunction _buttonUp = 0;
};

class CallbackObject
{
	friend Callback;

public:
	CallbackObject();
	virtual ~CallbackObject();

	void addCallback(const EventCallback event, CallbackFunction function);

private:
	CallbackHandlerPtr _callbackHandler;
};

class Callback
{
	friend CallbackObject;

public:
	static void tap_down();
	static void tap_pinch();
	static void tap_up();
	static void move(const float *pos);
	static void hint();

	static void buttonDown(unsigned int indexChar);
	static void buttonPinch();
	static void buttonUp(unsigned int indexChar);

public:
	static glm::vec2 pos;
	static glm::vec2 vector;

	static bool key[CALLBACK_COUNT_KEY];
	static unsigned int countPressButtons;
	static unsigned int charButtonDown;
	static unsigned int charButtonUp;

private:
	static void iteratintgCallbackHandler(const EventCallback event);

private:
	static unordered_map<int, CallbackHandlerPtr> _objects;
};
