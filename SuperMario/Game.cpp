#include "Game.h"
#include "Resources.h"
#include <iostream>
#include <filesystem>
#include "Map.h"
#include "Mario.h"

Map map(0.02f);
Camera camera(0.5f);

Mario mario;

void Begin(const sf::Window& window)
{
	//load textures
	for (auto& file : std::filesystem::directory_iterator("./Assets/Textures/")) {
		if (file.is_regular_file() && (file.path().extension() == ".png" || file.path().extension() == ".jpg")) {
			if (!Resources::textures[file.path().filename().string()].loadFromFile(file.path().string())) {
				std::cout << "Load texture failed\n";
			}
			else {
				std::cout << "Load texture succeed\n";
			}
		}
	}
	//load character
	for (auto& file : std::filesystem::directory_iterator("./Assets/Character/")) {
		if (file.is_regular_file() && (file.path().extension() == ".png" || file.path().extension() == ".jpg")) {
			if (!Resources::textures[file.path().filename().string()].loadFromFile(file.path().string())) {
				std::cout << "Load character failed\n";
			}
			else {
				std::cout << "Load character succeed\n";
			}
		}
	}
	//Generate map
	//map.CreateCheckerBoard(4, 2);
	sf::Image map_image;
	map_image.loadFromFile("./Assets/map.png");
	mario.position = map.CreateFromImage(map_image);
}

void Update(float deltaTime) {
	mario.Update(deltaTime);
	camera.position = mario.position;
}

void Render(Renderer& renderer) {
	map.Draw(renderer);
	mario.Draw(renderer);
}