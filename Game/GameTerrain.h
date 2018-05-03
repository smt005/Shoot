#pragma once

#include "Game.h"
//#include "Callback/Callback.h"

#include <memory>

class Map;
typedef std::shared_ptr<Map> MapPtr;

class Camera;

class GameTerrain : public Game// , public Callback
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
	void addObject(char* name);
};
