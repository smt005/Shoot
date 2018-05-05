#include "Game.h"
#include "GameTerrain.h"

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

void Game::onFrame()
{

}

void Game::draw()
{

}

Game* Game::getGame()
{
	if (!_game)
		_game = new GameTerrain();

	return _game;
}

void Game::saveGame()
{
	if (_game)
		_game->save();
}