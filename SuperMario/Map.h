#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Resources.h"
#include "Renderer.h"
class Map
{
public:
	Map(float c = 0.015f): cellSize(c), grid() {}
	void CreateCheckerBoard(int width, int height);
	void Draw(Renderer& renderer);
	sf::Vector2f CreateFromImage(const sf::Image& image);
	std::vector<std::vector<int>> grid;
	float cellSize;
};
