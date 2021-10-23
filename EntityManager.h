#pragma once
#include <memory>
#include <list>
#include <queue>
#include "Entity.h"
#include "EntityFactory.h"

using namespace std;

class EntityManager
{
private:
	EntityFactory entityFactory;
	list<shared_ptr<Entity>> _entities;
	queue<shared_ptr<Entity>> _newEntities;
public:
	// return entities matching desired components
	template<typename T, typename... TRest>
	list<shared_ptr<Entity>> get()
	{
		list<shared_ptr<Entity>> matchingEntities;
		for (shared_ptr<Entity> entity : _entities)
		{
			if ((*entity).has<T, TRest...>())
			{
				matchingEntities.push_back(entity);
			}
		}
		return matchingEntities;
	}
	template<typename E>
	Entity& make()
	{
		Entity& entity = entityFactory.make<E>();
		_newEntities.push(shared_ptr<Entity>(&entity));
		return entity;
	}
	void update()
	{
		// remove entities flagged as inactive
		for (auto iter = _entities.begin(); iter != _entities.end();)
		{
			if (!(**iter).active)
			{
				iter = _entities.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		// append new entities
		while (!_newEntities.empty())
		{
			_entities.push_back(_newEntities.front());
			_newEntities.pop();
		}
	}
	void remove(Entity& entity)
	{
		entity.active = false;
	}
	void remove(shared_ptr<Entity> entity)
	{
		this->remove(*entity);
	}
	// debug methods
	int numEntities()
	{
		return _entities.size();
	}
};