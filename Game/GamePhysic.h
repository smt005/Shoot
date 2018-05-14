#pragma once

#include "Game.h"
#include "../Engine/Callback/Callback.h"

#include <string>
#include <memory>
using namespace std;

class AiControll;
class Map;
typedef std::shared_ptr<Map> MapPtr;

class Camera;
class Callback;

class GamePhysic : public Game, public CallbackObject
{
private:
	MapPtr _mapPtr;
	AiControll* _ai;

	bool _visiblePhysic = false;
    
public:
	GamePhysic();
	~GamePhysic();

	bool init() override;
	void save() override;
	void update() override;
	void draw() override;

	void initMap();
	void initDraw();
	void initCallback();

private:
	bool close(void* data);
	bool rotateCamera(void *data);
	bool pressButton(void *data);
	bool pressButtonPinch(void *data);
	bool pressButtonDown(void *data);
	void addObject(const string& name);


	void controllCamera();
	void controllObject();
};
