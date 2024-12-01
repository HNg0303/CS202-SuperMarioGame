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

void Map::Draw(Renderer& renderer)
{
	//std::cout << "Drawing stuff\n";
	int x = 0;
	for (const auto& col : grid) {
		int y = 0;
		int s = col.size() / 2;
		for (const auto& cell : col) {
			if (cell == 1) {
				//std::cout << "cell = 1\n";
				sf::Vector2f cell_position(cellSize * x - cellSize * s, cellSize * y - cellSize * s);
				renderer.Draw(Resources::textures["block2.png"],cell_position,sf::Vector2f(cellSize, cellSize));
			}
			y++;
		}
		x++;
	}
}

sf::Vector2f Map::CreateFromImage(const sf::Image& image)
{
	grid.clear(); //
	grid = vector<vector<int>> (image.getSize().x, vector<int> (image.getSize().y, 0));

	sf::Vector2f marioPos{};

	for (int x = 0; x < grid.size(); x++) {
		for (int y = 0; y < grid[x].size(); y++) {
			sf::Color color = image.getPixel(x, y);
			if (color == sf::Color::Black) {
				grid[x][y] = 1;
			}
			else if (color == sf::Color::Red)
				marioPos = sf::Vector2f(cellSize * x / 4.0f , cellSize * y / 4.0f);
		}
	}
	return marioPos;
}
