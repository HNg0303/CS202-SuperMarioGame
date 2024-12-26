#pragma once
#include "../Headers/INCLUDE.h"
#include "../Headers/Physics.h"
#include "../Headers/entity.h"
#include "../Headers/Resources.h"
#include "../Headers/Renderer.h"


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
	int index;
public:
	int difficulty; // 0: easy. 1: medium, 2: hard
	int stage; // 0, 1, 2 
	bool isTheme = false;
	Map(float c, int d, int s = 0, int index = 0) : cellSize(c), grid(), difficulty(d), stage(s), index(index) {
		this->readObj("colorSheet.csv");
	}
	void CreateCheckerBoard(int width, int height);
	void Draw(Renderer& renderer);
	sf::Vector2f CreateFromImage(const sf::Image& image, vector<Entity*>& entites);
	void setMapIndex(int index);
	int getIndex();
	std::vector<std::vector<int>> grid;
	float cellSize;
	void Update(Vector2f pos);
	void setGrid(const sf::Image& image);
	void saveMapState(string filename, Vector2f& CharPos);
	void readObj(string filename);
	Entity* createEntityFromMap(int z, int x, int y);
	void drawBackground(Renderer& renderer, int n);
	void readMapState(string filename);
	~Map();
};
