#include "Character.h"
#include "Map.h"
#include "INCLUDE.h"
#include "Renderer.h"
#include "Camera.h"
#include "Game.h"


int main() {
	RenderWindow window(VideoMode(1200, 900), "Super Mario !!!");
	Clock deltaClock;
	Renderer renderer(window); //This is used to retain a renderer through out the loop
	Resources resource;
	Map map(32.0f);
	Camera camera(800.0f);
	Mario mario;

	resource.loadResource();

	/*
	Game* game = Game::getInstance(map, mario, camera);
	game->Begin(window);
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();
		game->Render(renderer, resource);
		//renderer.Draw(resource.getTexture("block2.png"), Vector2f(600.0f, 450.f), sf::Vector2f(200.0f, 200.0f), 0);
		//map.Draw(renderer, resource);
		window.display();
	}*/
	
	
	//Initialize Game instance => Should be an Game arrays of mementos or checkpointss here
	
	Game* game = Game::getInstance(map, mario, camera);
	if (game) cout << "YES!" << endl;
	game->Begin(window);
	while (window.isOpen()) {
		float deltaTime = deltaClock.restart().asSeconds();

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();
		game->Update(deltaTime, window);
		game->Render(renderer, resource);
		window.display();
	}
	
}