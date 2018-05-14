#include "Game.h"
#include "GameTerrain.h"
#include "GamePhysic.h"

Game *Game::_game = nullptr;

Game::Game()
{
	
}

Game::~Game()
{

}

bool Game::init()
{
	return true;
}

void Game::save()
{

}

void Game::update()
{

}

void Game::draw()
{

}

Game* Game::getGame()
{
	if (!_game)
		//_game = new GameTerrain();
		_game = new GamePhysic();

	return _game;
}

void Game::saveGame()
{
	if (_game)
		_game->save();
}