#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class Manager;

/*
size_t is shorthand for the unsigned int size of a container.
we may have LOTS of components so it is good to have a large supply of IDs.
*/
using ComponentID = std::size_t;

/*
Every time we call this, lastID will remember what it was last.
So when we return lastID++, we will get the next one.
*/
using Group = std::size_t;
inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u; // unsigned
	return lastID++;
}

/*
Gets a component's ID.
*/
template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

// Entities are not allowed to hold more than this many components
constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

/*
These two lines define a component array for an entity, which will
allow us to compare cap and compare components we already have so
that duplicates are not introduced
*/
using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

// +------------------------+
// | $$$ COMPONENT CLASS $$$|
// +------------------------+

class Component
{
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
	virtual ~Component() {}
};

// +---------------------+
// | $$$ ENTITY CLASS $$$|
// +---------------------+

class Entity
{
private:
	Manager& manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;

public:
	// Note: lowercase m :=member variable
	Entity(Manager& mManager) : manager(mManager) {}
	void update()
	{
		for (auto& c : components) c->update();
	}
	void draw()
	{
		for (auto& c : components) c->draw();
	}
	bool isActive() const { return active; }
	// ComponentManager will loop through all components and destroy those that are inactive
	void destroy() { active = false; }

	bool hasGroup(Group mGroup)
	{
		return groupBitSet[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		groupBitSet[mGroup] = false;
	}

	// Used during tests if component already exists
	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&...mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		/* When we get a specific kind of component c, it will
		always have the same position in the component array,
		based on its componentTypeID:
		*/
		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

// +----------------------+
// | $$$ MANAGER CLASS $$$|
// +----------------------+

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;
public:
	
	void update()
	{
		for (auto& e : entities) e->update();
	}
	void draw()
	{
		for (auto& e : entities) e->draw();
	}

	void refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			// erase() has only 2 arguments:
			v.erase(
				std::remove_if(std::begin(v), std::end(v), [i](Entity* mEntity)
			{
				return !mEntity->isActive() || !mEntity->hasGroup(i);
			}
			),
				std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities),
			std::end(entities), [](const std::unique_ptr<Entity> &mEntity)
		{
			return !mEntity->isActive();
		}),
			std::end(entities));
	}

	void addToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity); //append Entity to the end of group
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	Entity& addEntity()
	{
		Entity* e = new Entity(*this); // recieves reference to the manager object that gets created in the Game class
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};