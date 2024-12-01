#pragma once
#include <SFML/Graphics.hpp>
#include "INCLUDE.h"

class Renderer {
public:
	Renderer(sf::RenderTarget& t);
	void Draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size, float angle);
private:
	sf::Sprite sprite{};
	sf::RenderTarget& target;
};