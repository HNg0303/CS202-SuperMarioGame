#include "Map.h"
#include <iostream>

void Map::CreateCheckerBoard(int width, int height)
{
	grid = vector<vector<int>>(width, vector<int>(height, 0));
	int last = 0;
	for (auto& col : grid) {
		for (auto& cell : col) {
			if (last == 0) cell = 1;
			else if (last == 1) cell = 0;
			last = cell;
		}
	}
	for (auto& col : grid) {
		for (auto& cell : col) {
			std::cout << cell << " ";
		}
		std::cout << "\n";
	}
}

void Map::Update() {
	for (auto& entity : onEntities) {
		if (entity->deleted) {
			this->grid[entity->getCoords().x][entity->getCoords().y] = 0;
		}
	}
}

void Map::Draw(Renderer& renderer, Resources& resource)
{

	/*
	int startX = std::max(0, static_cast<int>(view.getCenter().x - view.getSize().x / 2) / cellSize);
	int endX = std::min(gridWidth, static_cast<int>(view.getCenter().x + view.getSize().x / 2) / cellSize);
	int startY = std::max(0, static_cast<int>(view.getCenter().y - view.getSize().y / 2) / cellSize);
	int endY = std::min(gridHeight, static_cast<int>(view.getCenter().y + view.getSize().y / 2) / cellSize);

	for (int x = startX; x < endX; ++x) {
		for (int y = startY; y < endY; ++y) {
			if (grid[x][y] == 1) {
				sf::Vector2f position(cellSize * x, cellSize * y);
				renderer.Draw(resource.getTexture("block2.png"), position, sf::Vector2f(cellSize, cellSize), 0);
			}
		}
	}*/
	
	//std::cout << "Drawing stuff\n";
	int x = 0;
	for (const auto& col : grid) {
		int y = 0;
		for (const auto& cell : col) {
			if (cell == 1) {
				//std::cout << "cell = 1\n";
				//sf::Vector2f cell_position(cellSize * x - cellSize * s, cellSize * y - cellSize * s);
				Vector2f cell_position(cellSize * x + cellSize / 2.0f, cellSize * y + cellSize / 2.0f);

				renderer.Draw(resource.getTexture("block2.png"),cell_position,sf::Vector2f(cellSize, cellSize), 0, 0);
			}
			y++;
		}
		x++;
	}
}

sf::Vector2f Map::CreateFromImage(const sf::Image& image, vector<Entity*>& entities)
{
	entities.clear();
	grid.clear(); 
	grid = vector<vector<int>> (image.getSize().x, vector<int> (image.getSize().y, 0));
	cout << endl << image.getSize().x << " " << image.getSize().y << endl;

	sf::Vector2f marioPos{};

	for (int x = 0; x < grid.size(); x++) {
		for (int y = 0; y < grid[x].size(); y++) {
			sf::Color color = image.getPixel(x, y);
			if (color == sf::Color::Black) {
				grid[x][y] = 1;
				Entity* block = new Block("block", 0.0, (cellSize * x + cellSize / 2.0f), (cellSize * y + cellSize / 2.0f), Vector2f(cellSize, cellSize), Vector2f(x,y));
				entities.push_back(block);
			}
			else if (color == sf::Color::Red)
				marioPos = sf::Vector2f(cellSize * x, cellSize * y);
			else if (color == sf::Color::Yellow) {
				grid[x][y] = 2;
				Entity* coin = new Coin("coin", 0.3, (cellSize * x + cellSize / 2.0f), (cellSize * y + cellSize / 2.0f), Vector2f(cellSize, cellSize), Vector2f(x, y));
				entities.push_back(coin);
			}
			else if (color == sf::Color::Green) {
				grid[x][y] = 3;
				Entity* goomba = new Enemy("goombas", 0.5f, 1.0f, (cellSize * x - cellSize / 2.0f), (cellSize * x + 2 * cellSize / 2.0f), (cellSize * y + cellSize / 2.0f), Vector2f(cellSize, cellSize), Vector2f(x, y));
				entities.push_back(goomba);
			}
		}
	}
	return marioPos;
}
