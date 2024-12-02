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
	Map map(0.02f);
	Camera camera(0.05f);
	Mario mario;

	resource.loadResource();

	/*
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();
		mario.Draw(renderer, 0, resource);
		window.display();
	}*/ //Testing
	
	
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
		window.setView(camera.GetView(window.getSize()));
		//game->Update(deltaTime);
		game->Render(renderer, resource);
		window.display();
	}
	
}