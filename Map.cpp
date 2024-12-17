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

				renderer.Draw(resource.getTexture("block2.png"),cell_position,sf::Vector2f(cellSize, cellSize), 0);
			}
			y++;
		}
		x++;
	}
}

sf::Vector2f Map::CreateFromImage(const sf::Image& image, vector<Entity*>& entities)
{
	entities.clear();
	/*grid.clear(); 
	grid = vector<vector<int>> (image.getSize().x, vector<int> (image.getSize().y, 0));*/
	this->setGrid(image);
	cout << endl << image.getSize().x << " " << image.getSize().y << endl;

	sf::Vector2f marioPos{};

	for (int x = 0; x < grid.size(); x++) {
		for (int y = 0; y < grid[x].size(); y++) {
			/*sf::Color color = image.getPixel(x, y);
			if (color == sf::Color::Black) {
				Entity* block = new Block("block", 0.0, (cellSize * x + cellSize / 2.0f), (cellSize * y + cellSize / 2.0f), Vector2f(cellSize, cellSize));
				entities.push_back(block);
			}
			else if (color == sf::Color::Red)
				marioPos = sf::Vector2f(cellSize * x, cellSize * y);
			else if (color == sf::Color::Yellow) {
				Entity* coin = new Coin("coin", 0.3, (cellSize * x + cellSize / 2.0f), (cellSize * y + cellSize / 2.0f), Vector2f(cellSize, cellSize));
				entities.push_back(coin);
			}
			else if (color == sf::Color::Green) {
				Entity* goomba = new Enemy("goombas", 0.5f, 1.0f, (cellSize * x - cellSize / 2.0f), (cellSize * x + 2 * cellSize / 2.0f), (cellSize * y + cellSize / 2.0f), Vector2f(cellSize, cellSize));
				entities.push_back(goomba);
			}*/
			int z = grid[x][y];
			Entity* entity = nullptr;
			if (z == GridColor::mario) {
				startingPos.first = x;
				startingPos.second = y;
				marioPos = sf::Vector2f(cellSize * x, cellSize * y);
			}
			else {
				entity = this->createEntityFromMap(z, x, y);
				if(entity != nullptr)
					entities.push_back(entity);
			}
		}
	}
	return marioPos;
}

void Map::setGrid(const sf::Image& image)
{
	grid.clear();
	grid = vector<vector<int>>(image.getSize().x, vector<int>(image.getSize().y, 0));
	cout << endl << image.getSize().x << " " << image.getSize().y << endl;
	for (int x = 0; x < grid.size(); x++) {
		for (int y = 0; y < grid[x].size(); y++) {
			sf::Color color = image.getPixel(x, y);
			for (int i = 0; i < objName.size(); i++) {
				if (color == objectMap[objName[i]]) {
					grid[x][y] = i + 1; //GridColor::objName[i] sadly doesn't work
					break;
				}
			}
		}
	}
}

void Map::saveMapState(string filename, Character* character)
{
	ofstream fout(filename);
	if (!fout.is_open()) {
		cout << "ERROR: CAN't open " + filename << endl;
		return;
	}
	sf::Vector2f endingPos = character->getPos();
	pair<int, int> endingPosInGrid;
	endingPosInGrid.first = floor(endingPos.x);
	endingPosInGrid.second = floor(endingPos.y);
	if (startingPos != endingPosInGrid)
		grid[startingPos.first][startingPos.second] = GridColor::nothing;
	grid[endingPosInGrid.first][endingPosInGrid.second] = GridColor::mario;
	for (int x = 0; x < grid.size(); x++) {
		for (int y = 0; y < grid[x].size(); y++) {
			fout << grid[x][y];
			if (y != grid[x].size() - 1) fout << ",";
		}
		fout << endl;
	}
	fout.close();
}

vector<string> tokenize(string s, string del) {
	vector<string> tmp;
	int start = 0, end = -1 * del.size();
	do {
		start = end + del.size();
		end = s.find(del, start);
		tmp.push_back(s.substr(start, end - start));
	} while (end != -1);
	return tmp;
}

void Map::readObj(string filename)
{
	ifstream fin(filename);
	string buffer;
	vector<string> tmp;
	if (!fin.is_open()) {
		cout << "Error: Can't open file\n";
		return;
	}
	while (getline(fin, buffer)) {
		tmp = tokenize(buffer, ",");
		objName.push_back(tmp[0]);
		int R = stoi(tmp[1]);
		int G = stoi(tmp[2]);
		int B = stoi(tmp[3]);
		int A = stoi(tmp[4]);
		objectMap[tmp[0]] = sf::Color(R, G, B, A);
		for (int i = 0; i < 5; i++) cout << tmp[i] << " ";
		cout << endl;
	}
	fin.close();
}

Entity* Map::createEntityFromMap(int z, int x, int y)
{
	Entity* entity = nullptr;
	if (z == GridColor::tile)
		entity = new Block("block", 0.0, (cellSize * x + cellSize / 2.0f), (cellSize * y + cellSize / 2.0f), Vector2f(cellSize, cellSize));
	if (z == GridColor::coin)
		entity = new Coin("coin", 0.3, (cellSize * x + cellSize / 2.0f), (cellSize * y + cellSize / 2.0f), Vector2f(cellSize, cellSize));
	if (z == GridColor::shell)
		entity = new Enemy("goombas", 0.5f, 1.0f, (cellSize * x - cellSize / 2.0f), (cellSize * x + 2 * cellSize / 2.0f), (cellSize * y + cellSize / 2.0f), Vector2f(cellSize, cellSize));
	return entity;
}

void Map::readMapState(string filename)
{
	grid.clear();
	ifstream fin(filename);
	if (!fin.is_open()) {
		cout << "Error: Can't open " + filename + " to read" << endl;
		return;
	}
	string buffer;
	vector<string> tmp;
	while (getline(fin, buffer)) {
		tmp = tokenize(buffer, ",");
		if (tmp.size() < 4) break;
		vector<int> col(tmp.size(), 0);
		for (int i = 0; i < col.size(); i++)
			col[i] = stoi(tmp[i]);
		grid.push_back(col);
	}
	fin.close();
}

