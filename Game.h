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
	vector<vector<string>> mapPaths; //to access a mapPath: mapPaths[map->difficulty][map->stage]
	
public:
	Game(Map* map, Character* character, Camera* camera);
	~Game();
	sf::View view;
	bool win = false;
	bool lose = false;
	Entity* getEntity(string name);
	void InitMapPath(){
		for (int i = 1; i <= 3; i++) {
			vector<string> tmp;
			for (int j = 1; j <= 3; j++) {
				string mapPath = "/Resource/Map/map" + to_string(i) + "-" + to_string(j) + ".png"; //Ex: /Resource/Map/map1-1.png
				tmp.push_back(mapPath);
			}
			mapPaths.push_back(tmp);
		}
	}
	void chooseCharacter(CharacterType type);
	void Begin(sf::RenderWindow& window);
	void Update(float& deltaTime, RenderWindow& window);
	void Render(Renderer& renderer, Resources& resource);
	int getCoin();
};
