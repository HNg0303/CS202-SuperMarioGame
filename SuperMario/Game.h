#pragma once
#include "Entity.h"
#include "Map.h"
#include "Resources.h"
#include "Camera.h"
#include "Renderer.h"
#include "Physics.h"
#include "Character.h"


class Game {
private:
	//inline static Game* instance = nullptr;
	Map* map = nullptr;
	Character* character = nullptr;
	Camera* camera = nullptr;
	Vector2f startPos{};
	sf::Image map_image;
	vector<string> mapPaths{ "/Resource/map1ColorProcessed.png", "/Resource/map2ColorProcessed.png", "/Resource/map3ColorProcessed.png" };
	//vector<Entity*> entities;
	//On screen
	
public:
	Game(Map* map, Character* character, Camera* camera);
	~Game();
	sf::View view;
	bool ended = false;
	//static vector<Entity*> onEntities;
	//static void deleteEntities(Entity* entity);
	Entity* getEntity(string name);
	//void loadEntities(vector<unique_ptr<Entity>> &entities);
	void chooseCharacter(CharacterType type);
	//static Game* getInstance(Map* map, Character* character, Camera* camera, bool isRestart = false);
	void Begin(sf::RenderWindow& window);
	void Update(float& deltaTime, RenderWindow& window);
	void Render(Renderer& renderer, Resources& resource);
	int getCoin();
};
