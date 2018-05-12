#include "GameTerrain.h"
#include "AiControll.h"

#include "../App/App.h"
#include "../App/File.h"

#include "../Engine/Draw/DrawEngine.h"
#include "../Engine/Draw/Camera.h"
#include "../Engine/Object/Map.h"
#include "../Engine/Object/Glider.h"
#include "../Engine/Object/Object.h"
#include "../Engine/Object/Model.h"
#include "../Engine/Object/Shape.h"
#include "../Engine/Object/Gun.h"
#include "../Engine/Common/Help.h"
#include "../Engine/Common/IncludesMatem.h"

#include "../Engine/Common/Log.h"

GameTerrain::GameTerrain()
{
}

GameTerrain::~GameTerrain()
{
}

bool GameTerrain::init()
{
	initMap();
	initDraw();
	initCallback();

	testFunction();

	return true;
}

void GameTerrain::save()
{
}

void GameTerrain::update()
{
	_mapPtr->action();

	Glider& glider = _mapPtr->getGliderByName("Glider_player");
	vec3 pos = glider.getPos();
	pos.z += 5.0f;
	Camera::current.setPos(pos);

	vec3 posCursor = Camera::current.corsorCoordZ();

	Object& object = help::find(_mapPtr->getObjects(), "MapArena");
	object.setPos(posCursor);

	if (_ai)
	{
		Glider* glider = _ai->getGlider();
		if (glider)
		{
			vec3 posGlider = glider->getPos();
			vec3 posCursor = Camera::current.corsorCoordZ();

			vec3 vector(normalize(posCursor - posGlider));
			_ai->setVector(vector);
		}
	}
}

void GameTerrain::draw()
{
	DrawEngine::prepareDraw(true);

	if (!_visiblePhysic)
	{
		DrawEngine::drawMap(*_mapPtr);
        
        if (_visibleVectorLight)
        {
            DrawEngine::prepareDrawLine();
            vector<Object*>& objects = _mapPtr->_objects;

            for (auto object : objects)
            {
                const glm::mat4x4& mat = object->getMatrix();

                float posObject[] = { mat[3][0], mat[3][1] , mat[3][2] };
                const float* lightDirect = DrawEngine::getLightDirect();
                float lenghtLine = 10.0f;
                float lightVector[] = { (posObject[0] - lightDirect[0] * lenghtLine), (posObject[1] - lightDirect[1] * lenghtLine), (posObject[2] - lightDirect[2] * lenghtLine) };
                float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
                DrawEngine::drawLine(posObject, lightVector, color);
            }
        }
	}
	else
	{
		DrawEngine::drawMapPhysic(*_mapPtr);
	}
}

void GameTerrain::initMap()
{
	_mapPtr = Map::getByName("MapGameTerrain");
	_mapPtr->setPhysic();
	Physics::setGravity(vec3(0.0f, 0.0f, 0.0f));

	// Glider
	Glider& glider = _mapPtr->getGliderByName("Glider_player");

	GliderTemplate* gliderTemplate = new GliderTemplate();
	gliderTemplate->_maxHeight = 4.0f;
	gliderTemplate->_minHeight = 3.0f;
	gliderTemplate->_speed = 0.02f;
	gliderTemplate->_speedRotate = 0.05f;
	glider.setTemplate(GliderTemplatePtr(gliderTemplate));

	_ai = new AiControll();
	glider.setAi(_ai);

	Object& object = _mapPtr->addObject("Box001");
	object.setName("testbox");
}

void GameTerrain::initDraw()
{
	DrawEngine::setBackgroundColor(0.3f, 0.6f, 0.9f, 1.0f);
	DrawEngine::setAmbientColor(0.2f, 0.2f, 0.3f);
	DrawEngine::initDrawMap();
	DrawEngine::initDrawLines();

	Camera::current.setDefault();
	Camera::current.setSpeed(0.1f);
	Camera::current.setCalcFrustum(false);

	//Camera::current.setFromEye(false);
	Camera::current.setDist(25.0f);
	//Camera::current.setVector(vec3(1.0f, 1.0f, -1.0f));
}

void GameTerrain::initCallback()
{
	//addCallback(EventCallback::TAP_PINCH, Function(rotateCamera));
	addCallback(EventCallback::MOVE, Function(rotateCamera));

	addCallback(EventCallback::TAP_PINCH, Function(shoot));
	addCallback(EventCallback::BUTTON_UP, Function(pressButton));
	addCallback(EventCallback::BUTTON_PINCH, Function(pressButtonPinch));
	addCallback(EventCallback::BUTTON_DOWN, Function(pressButtonDown));
}

bool GameTerrain::close(void* data)
{
	App::close();
	return true;
}

bool GameTerrain::shoot(void *data)
{
	if (_ai)
		_ai->setCommand(GliderCommand::SHOOT);

	return true;
}

bool GameTerrain::rotateCamera(void *data)
{
	Camera::current.rotate(Callback::vector);

	//_ai->setVector(Camera::current.vector());

	return true;
}

bool GameTerrain::pressButton(void *data)
{
	if (Callback::charButtonUp == VK_ESCAPE)
	{
		App::close();
		return true;
	}

	if (Callback::charButtonUp == VK_SPACE)
	{
		addObject("Dumbbell_05");
		return true;
	}

    if (Callback::charButtonUp == 'L')
    {
        _visibleVectorLight = !_visibleVectorLight;
    }
    
	return false;
}

bool GameTerrain::pressButtonPinch(void *data)
{
	controlGlider();
	controllCamera();

	return true;
}

bool GameTerrain::pressButtonDown(void *data)
{
	if (Callback::charButtonDown == VK_SPACE)
	{
		addObject("Dumbbell_0125");
		return true;
	}


	if (Callback::key[VK_CONTROL] && Callback::key['P'])
	{
		_visiblePhysic = !_visiblePhysic;
	}

	return false;
}

void GameTerrain::addObject(const string& name)
{
	glm::vec3 randomPos;
	randomPos.x = help::random_f(-50.0f, 100.0f);
	randomPos.y = help::random_f(-50.0f, 100.0f);
	randomPos.z = help::random_f(10.0f, 100.0f);

	_mapPtr->addObjectToPos(name, PhysicType::CONVEX, randomPos);
}

void GameTerrain::controlGlider()
{
	if (Callback::key[VK_CONTROL])
		return;

	if (!_ai)
		return;

	if (Callback::key['W'])
	{
		_ai->setCommand(GliderCommand::FOWARD);
	}

	if (Callback::key['S'])
	{
		_ai->setCommand(GliderCommand::BACK);
	}

	if (Callback::key['A'])
	{
		_ai->setCommand(GliderCommand::LEFT);
	}

	if (Callback::key['D'])
	{
		_ai->setCommand(GliderCommand::RIGHT);
	}

	if (Callback::key['R'])
	{
	}

	if (Callback::key['F'])
	{

	}
}

void GameTerrain::controllCamera()
{
	if (!Callback::key[VK_CONTROL])
		return;

	float speedCamera = 1.0f;
	if (Callback::key[VK_SHIFT])
	{
		speedCamera = 0.125f;
	}

	if (Callback::key['W'])
	{
		Camera::current.move(CAMERA_FORVARD, speedCamera);
	}

	if (Callback::key['S'])
	{
		Camera::current.move(CAMERA_BACK, speedCamera);
	}

	if (Callback::key['A'])
	{
		Camera::current.move(CAMERA_RIGHT, speedCamera);
	}

	if (Callback::key['D'])
	{
		Camera::current.move(CAMERA_LEFT, speedCamera);
	}

	if (Callback::key['R'])
	{
		Camera::current.move(CAMERA_TOP, speedCamera);
	}

	if (Callback::key['F'])
	{
		Camera::current.move(CAMERA_DOWN, speedCamera);
	}
}

class ClassTestBase
{
	string strB;
public:
	virtual void fB() {};
};

class ClassTest : public Object
{
	GliderTemplatePtr _template;
	AIptr _ai;
	GunPtr _gunPtr;

	bool _live = true;
	float _speedHeight;
	float _speed = 0.0f;
	vec3 _needVector;
	bool _commands[GLIDER_COUNT_COMMAND];

	virtual void f() {};
};

void GameTerrain::testFunction()
{
	LOG("\n");

	LOG_SIZE_STRUCT("ClassTestBase", ClassTestBase());
	LOG_SIZE_STRUCT("ClassTest", ClassTest());

	LOG("\n");

	LOG_SIZE_STRUCT("int", int());
	LOG_SIZE_STRUCT("double", double());

	LOG("\n");

	LOG_SIZE_STRUCT("string", string());
	LOG_SIZE_STRUCT("vec3", vec3());
	LOG_SIZE_STRUCT("mat4x4", mat4x4());
	
	LOG("\n");

	LOG_SIZE_STRUCT("ShapePtr", ShapePtr(new Shape()));
	LOG_SIZE_STRUCT("shared_ptr<Shape>", shared_ptr<Shape>(new Shape()));
	LOG_SIZE_STRUCT("shared_ptr<int>", shared_ptr<int>(new int()));

	LOG("\n");

	LOG_SIZE_STRUCT("Texture", Texture());
	LOG_SIZE_STRUCT("MeshPhysic", MeshPhysic());
	LOG_SIZE_STRUCT("PhysicObject", PhysicObject());
	LOG_SIZE_STRUCT("Model", Model());
	LOG_SIZE_STRUCT("Mesh", Mesh());
	LOG_SIZE_STRUCT("Shape", Shape());
	LOG_SIZE_STRUCT("Object", Object());
	LOG_SIZE_STRUCT("Glider", Glider());
	LOG_SIZE_STRUCT("Gun", Gun());
	LOG_SIZE_STRUCT("Map", Map());

	LOG("\n");
}