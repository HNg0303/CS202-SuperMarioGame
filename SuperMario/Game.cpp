#include "Game.h"

Game::Game(Map& map, Character* character, Camera& camera) : map(map), character(character), camera(camera) {
};

Game :: ~Game() {
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

Game* Game::getInstance(Map& map, Character* character, Camera& camera) {
	if (instance == nullptr) {
		instance = new Game(map, character, camera);
	}
	return instance;
}

void Game :: Begin(sf::RenderWindow& window)
{
	//Generate map
	//map.CreateCheckerBoard(4, 2);
	
	Physics::Init();

	sf::Image map_image;
	string mapPath = convertToUnixPath(fs::current_path().string()) + "/Resource/map.png";
	map_image.loadFromFile(mapPath);
	character->position = map.CreateFromImage(map_image);
	character->Begin();
	window.setView(camera.GetView(window.getSize()));
}

void Game :: Update(float deltaTime, RenderWindow& window) {
	Physics::Update(deltaTime);
	character->Update(deltaTime);
	camera.position = character->getPos();
	window.setView(camera.GetView(window.getSize()));
}

void Game :: Render(Renderer& renderer, Resources& resource) {
	map.Draw(renderer, resource);
	character->Draw(renderer, 0, resource);

	Physics::draw(renderer);
}