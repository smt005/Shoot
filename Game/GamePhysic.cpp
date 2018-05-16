#include "GamePhysic.h"
#include "AiControll.h"
#include "AiTarget.h"

#include "../App/App.h"
#include "../App/File.h"

#include "../Engine/Draw/DrawEngine.h"
#include "../Engine/Draw/Camera.h"
#include "../Engine/Object/Map.h"
#include "../Engine/Object/Object.h"
#include "../Engine/Object/Model.h"
#include "../Engine/Object/Shape.h"
#include "../Engine/Glider/Glider.h"
#include "../Engine/Glider/GliderTemplate.h"
#include "../Engine/Glider/Gun.h"
#include "../Engine/Effect/EffectObject.h"
#include "../Engine/Common/Help.h"
#include "../Engine/Common/IncludesMatem.h"

#include "../Engine/Common/JsonHelp.h"
#include "../Engine/Common/Log.h"

GamePhysic::GamePhysic()
{
}

GamePhysic::~GamePhysic()
{
}

bool GamePhysic::init()
{
	initMap();
	initDraw();
	initCallback();

	return true;
}

void GamePhysic::save()
{
	Camera::current.save();
}

void GamePhysic::update()
{
	_mapPtr->action();

	Object& trigger = _mapPtr->getObjectByName("trigger");
	PhysicObjectWptr& physicObjectW = trigger.getPhysic();
	if (physicObjectW.expired())
		return;

	PhysicObjectPtr physicObject = physicObjectW.lock();

	if (physicObject->getHasCollision())
	{
		trigger.setModel("sphereRed");
	}
	else
	{
		trigger.setModel("sphereGreen");
	}
}

void GamePhysic::draw()
{
	DrawEngine::prepareDraw(true);
	DrawEngine::drawMap(*_mapPtr);        
}

void GamePhysic::initMap()
{
	_mapPtr = Map::getByName("MapGamePhysic");
	Map::setCurrent(_mapPtr);
	_mapPtr->setPhysic();
	Physics::setGravity(vec3(0.0f, 0.0f, 0.0f));

	//Object& object = _mapPtr->getObjectByName("triangle");
	//object.setPhysic();
}


void GamePhysic::initDraw()
{
	DrawEngine::setBackgroundColor(0.3f, 0.6f, 0.9f, 1.0f);
	DrawEngine::setAmbientColor(0.2f, 0.2f, 0.3f);
	DrawEngine::initDrawMap();
	DrawEngine::initDrawLines();

	Camera::current.setDefault();
	Camera::current.setSpeed(1.0f);

	//Camera::current.setLookAt(vec3(-5.624f, -2.862f, -0.1f), vec3(30.2f, 2.86f, 0.25f));

	Camera::current.load();

	/*Camera::current.setDist(5.0f);
	Camera::current.setFromEye(true);
	Camera::current.setVector(vec3(1.0f, 0.0f, -1.0f));*/
}

void GamePhysic::initCallback()
{
	addCallback(EventCallback::TAP_PINCH, Function(rotateCamera));

	addCallback(EventCallback::BUTTON_UP, Function(pressButton));
	addCallback(EventCallback::BUTTON_PINCH, Function(pressButtonPinch));
	addCallback(EventCallback::BUTTON_DOWN, Function(pressButtonDown));
}

bool GamePhysic::close(void* data)
{
	App::close();
	return true;
}

bool GamePhysic::rotateCamera(void *data)
{
	Camera::current.rotate(Callback::vector);
	return true;
}

bool GamePhysic::pressButton(void *data)
{
	if (Callback::charButtonUp == VK_ESCAPE)
	{
		App::close();
		return true;
	}

    if (Callback::charButtonUp == 'G')
	{
		vec3 g = Physics::getGravity();

		if (g.z == 0.0f)
		{
			Physics::setGravity(vec3(0.0f, 0.0f, -0.0025f));
		}
		else
		{
			Physics::setGravity(vec3(0.0f, 0.0f, 0.0f));

			Object& trigger = _mapPtr->getObjectByName("trigger");
			vec3 pos = trigger.getPos();
			pos.z = 0.1f;
			trigger.setPos(pos);
			trigger.setSpeed(0.0f);
		}
	}

	if (Callback::charButtonUp == 'P')
	{
	}

	return false;
}

bool GamePhysic::pressButtonPinch(void *data)
{
	controllCamera();
	controllObject();

	return true;
}

bool GamePhysic::pressButtonDown(void *data)
{
	if (Callback::charButtonDown == VK_SPACE)
	{
		return true;
	}

	return false;
}

void GamePhysic::addObject(const string& name)
{
	glm::vec3 randomPos;
	randomPos.x = help::random_f(-50.0f, 100.0f);
	randomPos.y = help::random_f(-50.0f, 100.0f);
	randomPos.z = help::random_f(10.0f, 100.0f);

	_mapPtr->addObjectToPos(name, PhysicType::CONVEX, randomPos);
}

void GamePhysic::controllCamera()
{
	if (Callback::key[VK_CONTROL])
		return;

	if (Callback::key['O'])
		return;

	float speedCamera = Callback::key[VK_SHIFT] ? 0.06125f : speedCamera = 0.125f;
	Camera::current.setSpeed(speedCamera);

	if (Callback::key['Q'])
	{
		if (Callback::key['W'])
		{
			Camera::current.move(CameraMove::FORVARD);
		}

		if (Callback::key['S'])
		{
			Camera::current.move(CameraMove::BACK);
		}
	}

	if (Callback::key['W'])
	{
		Camera::current.move(CameraMove::HORIZONT);
	}

	if (Callback::key['S'])
	{
		Camera::current.move(CameraMove::BACK_HORIZONT);
	}

	if (Callback::key['A'])
	{
		Camera::current.move(CameraMove::RIGHT);
	}

	if (Callback::key['D'])
	{
		Camera::current.move(CameraMove::LEFT);
	}

	if (Callback::key['R'])
	{
		Camera::current.move(CameraMove::TOP);
	}

	if (Callback::key['F'])
	{
		Camera::current.move(CameraMove::DOWN);
	}
}

void GamePhysic::controllObject()
{
	if (!Callback::key['O'])
		return;

	Object& trigger = _mapPtr->getObjectByName("trigger");

	float speed = 0.01f;
	vec3 pos = trigger.getPos();

	if (Callback::key['W'])
	{
		pos.x += speed;
	}

	if (Callback::key['S'])
	{
		pos.x -= speed;
	}

	if (Callback::key['A'])
	{
		pos.y += speed;
	}

	if (Callback::key['D'])
	{
		pos.y -= speed;
	}

	if (Callback::key['R'])
	{
		pos.z += speed;	
	}

	if (Callback::key['F'])
	{
		pos.z -= speed;
		
	}

	trigger.setPos(pos);

	//---

	/*PhysicObjectWptr& physicObjectW = trigger.getPhysic();
	if (physicObjectW.expired())
		return;

	PhysicObjectPtr physicObject = physicObjectW.lock();

	if (physicObject->getHasCollision())
	{
		trigger.setModel("sphereRed");
	}
	else
	{
		trigger.setModel("sphereGreen");
	}*/

	//---

	/*Object& triangle = _mapPtr->getObjectByName("triangle");

	PhysicObjectWptr& physicObjectW = triangle.getPhysic();
	if (physicObjectW.expired())
		return;

	PhysicObjectPtr physicObject = physicObjectW.lock();*/

	/*int countPlanes = physicObject->getCountPlane();
	PhysicPlane* planes = physicObject->getPlanes();

	if (countPlanes == 0 || !planes)
		return;*/

	/*float dist = planes[0].distPointToPlane(pos);

	if (dist < 0.1)
	{
		bool entry = planes[0].entryPointToPlane(pos, dist, 0.01f);

		if (entry)
		{
			trigger.setModel("sphereRed");
		}
		else
		{
			trigger.setModel("sphereBlue");
		}
	}
	else
	{
		trigger.setModel("sphereGreen");
	}
	
	LOGI("LOG: dist: %f", dist);
	LOGI(" pos [%f", pos.x);
	LOGI(" %f", pos.y);
	LOGI(" %f] ", pos.z);

	LOG("\n");
	*/

	/*bool entry(false);

	for (int i = 0; i < countPlanes; ++i)
	{
		if (planes[i].entryPointToPlane(pos))
		{
			entry = true;
			break;
		}
	}

	if (entry)
	{
		trigger.setModel("sphereRed");
	}
	else
	{
		trigger.setModel("sphereGreen");
	}*/
}

