#include "Mario.h"
#include <iostream>
void Mario::Update(float deltaTime)
{
	float move = movementSpeed;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		move *= 2;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		//std::cout << "Keyboard is working\n";
		position.x += move * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		//std::cout << "Keyboard is working\n";
		position.x -= move * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		//std::cout << "Keyboard is working\n";
		position.y -= move * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		//std::cout << "Keyboard is working\n";
		position.y += move * deltaTime;
	}
}

void Mario::Draw(Renderer& renderer)
{
	renderer.Draw(Resources::textures["run1.png"], position, sf::Vector2f(width, height));
}
