#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Camera.h"
#include "Renderer.h"
#include "Map.h"

int main() {

	sf::RenderWindow window(sf::VideoMode(1600u, 900u), "MARIO");
	sf::Clock deltaClock;
	Renderer renderer(window);


	Begin(window);
	while (window.isOpen()) {
		float deltaTime = deltaClock.restart().asSeconds();
		sf::Event event{};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.setView(camera.GetView(window.getSize()));
		Update(deltaTime);
		window.clear(sf::Color(0, 0, 0));
		Render(renderer);
		window.display();
	}
}

