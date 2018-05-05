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

class GameTerrain : public Game, public CallbackObject
{
private:
	MapPtr _mapPtr;
	AiControll* _ai;

	bool _visiblePhysic = false;
    bool _visibleVectorLight = false;
    
public:
	GameTerrain();
	~GameTerrain();

	bool init() override;
	void save() override;
	void onFrame() override;
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

	void controlGlider();
	void controllCamera();
};
