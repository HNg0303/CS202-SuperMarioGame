#pragma once
#include "INCLUDE.h"
#include "Entity.h"
#include "Map.h"
#include "Resources.h"
#include "Camera.h"
#include "Renderer.h"
#include "Physics.h"
#include "Character.h"


class Game {
private:
	inline static Game* instance;
	Map map;
	Character* character = nullptr;
	Camera camera;
	Game(Map& map, Character* character, Camera& camera);
	vector<Entity*> entities;

	//On screen
	
	~Game();
public:
	//static vector<Entity*> onEntities;
	//static void deleteEntities(Entity* entity);
	Entity* getEntity(string name);
	void loadEntities(vector<unique_ptr<Entity>> &entities);
	void chooseCharacter(CharacterType type);
	static Game* getInstance(Map& map, Character* character, Camera& camera);
	void Begin(sf::RenderWindow& window);
	void Update(float& deltaTime, RenderWindow& window);
	void Render(Renderer& renderer, Resources& resource);
};
