#pragma once

#include "Game.h"
#include "../Engine/Callback/Callback.h"

#include <string>
#include <memory>
using namespace std;

class Map;
typedef std::shared_ptr<Map> MapPtr;

class Camera;
class Callback;

class GameTerrain : public Game, public Callback
{
private:
	MapPtr _map;
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
	bool rotateCamera(void *data);
	bool pressButton(void *data);
	bool pressButtonPinch(void *data);
	bool pressButtonDown(void *data);
	void addObject(const string& name);
};
