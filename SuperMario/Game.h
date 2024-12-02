#pragma once
#include <SFML/Graphics.hpp>
#include "INCLUDE.h"
#include "Map.h"
#include "Resources.h"
#include "Character.h"
#include "Camera.h"
#include "Renderer.h"

class Game {
private:
	inline static Game* instance;
	Map map;
	Mario mario;
	Camera camera;
	Game(Map& map, Mario& mario, Camera& camera);
	~Game();
public:
	static Game* getInstance(Map& map, Mario& mario, Camera& camera);
	void Begin(sf::RenderWindow& window);
	void Update(float deltaTime, RenderWindow& window);
	void Render(Renderer& renderer, Resources& resource);
};
