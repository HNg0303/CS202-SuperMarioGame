#include "GameFlow.h"

int main() {
	
	/*
	RenderWindow window(VideoMode(1200, 900), "Super Mario !!!");
	Clock deltaClock;
	Renderer renderer(window); //This is used to retain a renderer through out the loop
	Resources resource;
	resource.loadResource();*/

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
	//window.setFramerateLimit(60);
	GameFlow* flow = GameFlow::getInstance();
	flow->run();
}