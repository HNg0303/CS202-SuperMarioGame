#include "Game.h"
#include "SFML/Audio.hpp"

Music music{};

Game::Game(Map& map, Mario& mario, Camera& camera) : map(map), mario(mario), camera(camera) {};

Game :: ~Game() {
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

Game* Game::getInstance(Map& map, Mario& mario, Camera& camera) {
	if (instance == nullptr) {
		instance = new Game(map, mario, camera);
	}
	return instance;
}

void Game :: Begin(sf::RenderWindow& window)
{
	//Generate map
	//map.CreateCheckerBoard(4, 2);
	sf::Image map_image;
	string mapPath = convertToUnixPath(fs::current_path().string()) + "/Resource/map.png";
	map_image.loadFromFile(mapPath);
	music.openFromFile(convertToUnixPath(fs::current_path().string() + "/Resource/Music/music.wav"));
	music.setLoop(true);
	music.setVolume(30);
	mario.Begin();
	mario.position = map.CreateFromImage(map_image);
	window.setView(camera.GetView(window.getSize()));
	music.play();
}

void Game :: Update(float deltaTime, RenderWindow& window) {
	mario.Update(deltaTime);
	camera.position = mario.position;
	window.setView(camera.GetView(window.getSize()));
}

void Game :: Render(Renderer& renderer, Resources& resource) {
	map.Draw(renderer, resource);

	mario.Draw(renderer, 0, resource);
}