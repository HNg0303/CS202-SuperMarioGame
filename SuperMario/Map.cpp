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

void Map::Draw(Renderer& renderer, Resources& resource)
{

	//std::cout << "Drawing stuff\n";
	int x = 0;
	for (const auto& col : grid) {
		int y = 0;
		int s = col.size() / 2;
		for (const auto& cell : col) {
			if (cell == 1) {
				//std::cout << "cell = 1\n";
				//sf::Vector2f cell_position(cellSize * x - cellSize * s, cellSize * y - cellSize * s);
				Vector2f cell_position(cellSize * x + cellSize / 2.0f, cellSize * y + cellSize / 2.0f);

				renderer.Draw(resource.getTexture("block2.png"),cell_position,sf::Vector2f(cellSize, cellSize), 0);
			}
			y++;
		}
		x++;
	}
}

sf::Vector2f Map::CreateFromImage(const sf::Image& image)
{
	grid.clear(); 
	grid = vector<vector<int>> (image.getSize().x, vector<int> (image.getSize().y, 0));
	cout << endl << image.getSize().x << " " << image.getSize().y << endl;

	sf::Vector2f marioPos{};

	for (int x = 0; x < grid.size(); x++) {
		for (int y = 0; y < grid[x].size(); y++) {
			sf::Color color = image.getPixel(x, y);
			if (color == sf::Color::Black) {
				grid[x][y] = 1;
				//Create static tile in a box2D world through Physics class.
				b2BodyDef bodyDef;
				bodyDef.type = b2_staticBody;
				bodyDef.position.Set(cellSize * x + cellSize / 2.0f, cellSize * y + cellSize / 2.0f);
				b2Body* body = Physics::world.CreateBody(&bodyDef);
				b2PolygonShape shape;
				shape.SetAsBox(cellSize / 2.0f, cellSize / 2.0f);//Set at the center
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &shape;
				body->CreateFixture(&fixtureDef);
			}
			else if (color == sf::Color::Red)
				marioPos = sf::Vector2f(cellSize * x, cellSize * y);
		}
	}
	return marioPos;
}
