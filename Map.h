#pragma once
#include "INCLUDE.h"
#include "Physics.h"
#include "entity.h"
#include "Resources.h"
#include "Renderer.h"
/*
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
};*/

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
	int difficulty; // 0: easy. 1: medium, 2: hard
	int stage; // 0, 1, 2 
	bool isTheme;
	Map(float c, int d, int s = 0) : cellSize(c), grid(), difficulty(d), stage(s) { 
		this->readObj("colorSheet.csv");
	}
	void CreateCheckerBoard(int width, int height);
	void Draw(Renderer& renderer, Resources& resource);
	sf::Vector2f CreateFromImage(const sf::Image& image, vector<Entity*>& entites);
	std::vector<std::vector<int>> grid;
	float cellSize;
	void Update(Vector2f pos);
	void setGrid(const sf::Image& image);
	void saveMapState(string filename, Vector2f& CharPos);
	void readObj(string filename);
	Entity* createEntityFromMap(int z, int x, int y);
	void readMapState(string filename);
	void drawBackground(Renderer& renderer, int n);
	~Map();
};
