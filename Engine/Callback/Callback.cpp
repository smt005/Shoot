#include "Callback.h"
#include "../Common/Log.h"
#include "../App/App.h"

glm::vec2 Callback::pos;
glm::vec2 Callback::vector;
bool Callback::key[CALLBACK_COUNT_KEY];
unsigned int Callback::countPressButtons = 0;
unsigned int Callback::charButtonDown;
unsigned int Callback::charButtonUp;

unordered_map<int, CallbackHandlerPtr> Callback::_objects;

void CallbackHandler::set(const EventCallback event, const CallbackFunction &function)
{
	switch (event)
	{

	case EventCallback::TAP_DOWN:
		_tap_down = function;
		break;
	case EventCallback::TAP_PINCH:
		_tap_pinch = function;
		break;
	case EventCallback::TAP_UP:
		_tap_up = function;
		break;
	case EventCallback::TAP_LONG:
		_tap_long = function;
		break;
	case EventCallback::TAP_DOUBLE:
		_tap_double = function;
		break;
	case EventCallback::MOVE:
		_move = function;
		break;
	case EventCallback::HINT:
		_hint = function;
		break;
	case EventCallback::BUTTON_DOWN:
		_buttonDown = function;
		break;
	case EventCallback::BUTTON_PINCH:
		_buttonPinch = function;
		break;
	case EventCallback::BUTTON_UP:
		_buttonUp = function;
	}
}

void CallbackHandler::runFunction(const EventCallback event)
{
	switch (event)
	{
	case EventCallback::TAP_DOWN:
	{
		if (_tap)
			return;

		_tap = true;
		
		double time = App::getCurentTime();

		if (time - _time < 300)
		{
			if (_tap_double)
				_tap_double(this);
		}

		_time = time;

		if (_tap_down)
			_tap_down(this);
	} break;
	case EventCallback::TAP_PINCH:
	{
		if (!_tap)
			return;

		double time = App::getCurentTime();

		if (!_wasLongTap && time - _time > 500)
		{
			if (_tap_long)
				_tap_long(this);
			_wasLongTap = true;
		}

		if (_tap_pinch)
			_tap_pinch(this);
	} break;
	case EventCallback::TAP_UP:
	{
		if (!_tap)
			return;

		_tap = false;
		_wasLongTap = false;

		if (_tap_up)
			_tap_up(this);
	} break;
	case EventCallback::MOVE:
	{
		if (_move)
			_move(this);
	} break;
	case EventCallback::HINT:
	{

	} break;

	case EventCallback::BUTTON_DOWN:
	{
		if (_buttonDown)
			_buttonDown(this);
	} break;
	case EventCallback::BUTTON_PINCH:
	{
		if (_buttonPinch)
			_buttonPinch(this);
	} break;
	case EventCallback::BUTTON_UP:
	{
		if (_buttonUp)
			_buttonUp(this);
	}
	}
}

//---

CallbackObject::CallbackObject()
{
}

CallbackObject::~CallbackObject()
{
	if (!_callbackHandler)
		return;

	auto it = Callback::_objects.find(_callbackHandler->getId());

	if (it == Callback::_objects.end())
		return;

	Callback::_objects.erase(it);

	LOG("LOG: CallbackObject ~CallbackObject\n");
}

void CallbackObject::addCallback(const EventCallback event, CallbackFunction function)
{
	if (!_callbackHandler)
	{
		CallbackHandler* callbackHandler = new CallbackHandler();
		_callbackHandler = CallbackHandlerPtr(callbackHandler);
		Callback::_objects.emplace(_callbackHandler->getId(), _callbackHandler);
	}

	_callbackHandler->set(event, function);
}

//---

void Callback::tap_down()
{
	iteratintgCallbackHandler(EventCallback::TAP_DOWN);
}

void Callback::tap_pinch()
{
	iteratintgCallbackHandler(EventCallback::TAP_PINCH);
}

void Callback::tap_up()
{
	iteratintgCallbackHandler(EventCallback::TAP_UP);
}

void Callback::move(const float *newPos)
{
	if (pos.x == newPos[0] && pos.y == newPos[1])
	{
		vector.x = 0.0;
		vector.y = 0.0;
		return;
	}

	vector.x = (float)(pos.x - newPos[0]);
	vector.y = (float)(pos.y - newPos[1]);
	pos = glm::vec2(newPos[0], newPos[1]);

	iteratintgCallbackHandler(EventCallback::MOVE);
}

void Callback::hint()
{
	iteratintgCallbackHandler(EventCallback::HINT);
}

void Callback::buttonDown(unsigned int indexChar)
{
	if (indexChar >= CALLBACK_COUNT_KEY)
		return;

	if (key[indexChar] == true)
		return;

	Callback::key[indexChar] = true;
	charButtonDown = indexChar;
	++countPressButtons;

	iteratintgCallbackHandler(EventCallback::BUTTON_DOWN);

	charButtonDown = '\0';
}

void Callback::buttonPinch()
{
	if (countPressButtons > 0)
		iteratintgCallbackHandler(EventCallback::BUTTON_PINCH);
}

void Callback::buttonUp(unsigned int indexChar)
{
	if (indexChar >= CALLBACK_COUNT_KEY)
		return;

	if (key[indexChar] == false)
		return;

	Callback::key[indexChar] = false;
	charButtonUp = indexChar;
	--countPressButtons;

	iteratintgCallbackHandler(EventCallback::BUTTON_UP);

	charButtonUp = '\0';
}

void Callback::iteratintgCallbackHandler(const EventCallback event)
{
	for (unordered_map<int, CallbackHandlerPtr>::iterator it = _objects.begin(); it != _objects.end(); ++it)
	{
		CallbackHandlerPtr callbackHandlerPtr = it->second;

		if (callbackHandlerPtr)
			callbackHandlerPtr->runFunction(event);
	}
}