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
	//vector<string> mapPaths{ "/Resource/Map1-3/map1ColorProcessed.png", "/Resource/Map1-3/map2ColorProcessed.png", "/Resource/Map1-3/map3ColorProcessed.png" };
	vector<vector<string>> mapPaths; //to access a mapPath: mapPaths[map->difficulty][map->stage]
	//On screen
	
public:
	Game(Map* map, Character* character, Camera* camera);
	~Game();
	sf::View view;
	bool win = false;
	bool lose = false;
	Entity* getEntity(string name);
	void InitMapPath();
	void chooseCharacter(CharacterType type);
	void Begin(sf::RenderWindow& window);
	void Update(float& deltaTime, RenderWindow& window);
	void Render(Renderer& renderer, Resources& resource);
	int getCoin();
};
