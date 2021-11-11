#include <iostream>
#include <vector>
#include <string>
#include "Game.h"
#include "Component.h"
#include "Entity.h"
#include "ConcreteSystems.h"
#include "FileLoadFactory.h"

void init(Game& game)
{

	EntityFactory factory;
	game.enttMngr().insert(factory.make<Player>());
	
	std::map<string,Entity*> gameData = EntityFileLoader::load("levels/level_0", game);
	
	/*
	Spawner& spawner = gameSpawner.get<Spawner>();
	spawner.add(&factory.make<Asteroid>(float2{ 100,0 }, float2{ 0,2.1 }), 0);
	spawner.add(&factory.make<Asteroid>(float2{ 400,0 }, float2{ 0,2.3 }), 120);
	spawner.add(&factory.make<Asteroid>(float2{ 500,0 }, float2{ 0,2.2 }), 60);
	spawner.add(&factory.make<Asteroid>(float2{ 600,0 }, float2{ 0,1.7 }), 150);
	spawner.add(&factory.make<Asteroid>(float2{ 128,0 }, float2{ 0,2.5 }), 50);*/
	game.enttMngr().insert(*gameData["spawner"]);

	game.addSystem(new SysPlayerInput);
	game.addSystem(new SysJetAI);
	game.addSystem(new SysHeliAI);
	game.addSystem(new SysMove);
	game.addSystem(new SysMoveCamera);
	game.addSystem(new SysRender);
	game.addSystem(new SysUpdateSpawners);
	game.addSystem(new SysShipShoot);
	game.addSystem(new SysHitEnemyCollisions);
	game.addSystem(new SysDestroyNoHealth);
	game.addSystem(new SysDestroyOutOfBounds);
	
}


int main(int argc, char* args[])
{
	Entity* e = new Entity;
	Game game(768, 1024, "Shmup");
	init(game);
	while (game.isRunning())
	{
		game.update();
	}
	return 0;
}
