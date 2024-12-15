#pragma once
#include <SFML/Graphics.hpp>
#include "INCLUDE.h"

class Renderer {
public:
	RenderWindow* getRenderWindow();
	Renderer(sf::RenderTarget& t);
	sf::RenderTarget& target;
	void Draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size, float angle);
private:
	sf::Sprite sprite{};
	
};