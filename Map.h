#pragma once
#include "INCLUDE.h"
#include "Physics.h"
#include "entity.h"
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "Renderer.h"
#include "Character.h"
enum GridColor {
	nothing = 0, tile, mario, coin, block, stone, question, spike,
	pipe, plant, shell, powerup_red, powerup_green, spikyTurtle,
	goal, lava1, lava2, fireBar, peach, tileMap3, woodBlock, bridge, bowser
};

class Map
{
private:
	vector<string> objName;
	map<string, sf::Color> objectMap;
	pair<int, int> startingPos;
public:
	Map(float c = 0.015f): cellSize(c), grid() {
		this->readObj("colorSheet.csv");
	}
	void CreateCheckerBoard(int width, int height);
	void Draw(Renderer& renderer, Resources& resource);
	sf::Vector2f CreateFromImage(const sf::Image& image, vector<Entity*> &entites);
	std::vector<std::vector<int>> grid;
	float cellSize;
	void setGrid(const sf::Image& image);
	void saveMapState(string filename, Character* character);
	void readObj(string filename);
	Entity* createEntityFromMap(int z, int x, int y);
	void readMapState(string filename);
};

