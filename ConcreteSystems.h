#pragma once
#include <iostream>
#include <SDL_events.h>
#include "System.h"
#include "ConcreteComponents.h"

typedef list<shared_ptr<Entity>> Entities;

class SysPlayerInput : public System
{
public:
	void update() override 
	{ 
		Entities entities = entitySource->get<Player>();
		for (auto& entity : entities)
		{
			float2& vel = entity->get<Velocity>().velocity;
			const Uint8* keystate = SDL_GetKeyboardState(nullptr);

			// TO DO: Use key-mapping to make this prettier
			if (!(keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_DOWN]
				|| keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_S]))
			{
				vel.y = 0;
			}
			else
			{
				if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W]) { vel.y = -1; }
				if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) { vel.y = 1; }
			}
			if (!(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_RIGHT]
				|| keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_D]))
			{
				vel.x = 0;
			}
			else
			{
				if (keystate[SDL_SCANCODE_LEFT]
					|| keystate[SDL_SCANCODE_A]) { vel.x = -1; }
				if (keystate[SDL_SCANCODE_RIGHT]
					|| keystate[SDL_SCANCODE_D]) { vel.x = 1; }
			}
			if (keystate[SDL_SCANCODE_SPACE])
			{
				entity->get<Ship>().shoot = true;
			}
			else
			{
				entity->get<Ship>().shoot = false;
			}
			/*
			for (auto& input : gameInfo->sdlEvents)
			{
				switch (input.type)
				{
					case SDL_KEYDOWN:
						switch (input.key.keysym.sym)
						{
							case SDLK_UP:
								cout << "UP PRESSED" << endl;
								vel.y = -1;
								break;
							case SDLK_DOWN:
								cout << "DOWN PRESSED" << endl;
								vel.y = 1;
								break;
							case SDLK_LEFT:
								cout << "LEFT PRESSED" << endl;
								vel.x = -1;
								break;
							case SDLK_RIGHT:
								cout << "RIGHT PRESSED" << endl;
								vel.x = 1;
								break;
							default:
								break;
						}
						break;
					case SDL_KEYUP:
						switch (input.key.keysym.sym)
						{
							case SDLK_UP:
								cout << "UP RELEASED" << endl;
								vel.y = 0;
								break;
							case SDLK_DOWN:
								cout << "DOWN RELEASED" << endl;
								vel.y = 0;
								break;
							case SDLK_LEFT:
								cout << "LEFT RELEASED" << endl;
								vel.x = 0;
								break;
							case SDLK_RIGHT:
								cout << "RIGHT RELEASED" << endl;
								vel.x = 0;
								break;
							default:
								break;
						}
						break;
					default:
						break;
				}

			}*/
		}
	}
};

class SysMove : public System
{
public:
	void update() override
	{
		Entities entities = entitySource->get<Position, Velocity>();
		for (auto& entity : entities)
		{
			entity->get<Position>().position += entity->get<Velocity>().velocity;
		}
	}
};

class SysRender : public System
{
public:
	void update() override 
	{
		Entities entities = entitySource->get<TextureBox, Position, Dimensions>();
		for (auto& entity : entities)
		{
			float2& pos = entity->get<Position>().position;
			Dimensions& dim = entity->get<Dimensions>();
			entity->get<TextureBox>().texture.render(pos.x - (dim.w/2), pos.y - (dim.h/2), dim.w, dim.h);
		}
	}
};

class SysShipShoot : public System
{
public:
	void update() override
	{
		
		Entities entities = entitySource->get<Ship, Spawner>();
		for (auto& entity : entities)
		{
			Spawner& spawner = entity->get<Spawner>();
			spawner.active = entity->get<Ship>().shoot;
		}
	}
};
class SysUpdateSpawners : public System
{
public:
	// Updates each spawners validEntites list.
	void update() override
	{
		Entities entities = entitySource->get<Spawner>();
		for (auto& entity : entities)
		{
			Spawner& spawner = entity->get<Spawner>();

			// Check if spawner is even active
			if (spawner.active)
			{
				list<Entity*> toSpawn;
				// Keep "deqeuing" until spawner ticks != front pair ticks
				pair<Entity*, int> next;
				while ((next = spawner.sequence[spawner.index]).second == spawner.ticks)
				{

					// Clone entity
					Entity* copy = new Entity(*next.first);
					entitySource->insert(*copy);

					// Spawn entity by giving it a position
					if (spawner.origin != nullptr)
					{
						copy->add<Position>().position = *(spawner.origin);
					}

					// Update other spawner data
					spawner.ticks = 0;
					spawner.index = (spawner.index + 1) % spawner.sequence.size();
				}
				spawner.ticks++;
			}
			// else, reset spawner
			else
			{
				spawner.index = 0;
				spawner.ticks = 0;
			}
		}
	}
};