#pragma once

class Game
{
public:
	Game();
	virtual ~Game();

	virtual bool init();
	virtual void save();
	virtual void update();
	virtual void draw();

public:
	static Game* _game;
	static Game* getGame();
	static void saveGame();
};

