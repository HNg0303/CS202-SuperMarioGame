#pragma once
#include "INCLUDE.h"
#include "Physics.h"
#include "entity.h"
#include "Resources.h"
#include "Renderer.h"
class Map
{
public:
	void Update();
	Map(float c = 0.015f): cellSize(c), grid() {}
	void CreateCheckerBoard(int width, int height);
	void Draw(Renderer& renderer, Resources& resource);
	sf::Vector2f CreateFromImage(const sf::Image& image, vector<Entity*> &entites);
	std::vector<std::vector<int>> grid;
	float cellSize;
};
