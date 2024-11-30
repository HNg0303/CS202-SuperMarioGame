#include "Renderer.h"
#include <vector>
Renderer::Renderer(sf::RenderTarget& t) : target(t) {}

void Renderer::Draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size) {
	sprite.setTexture(texture, true);
	sf::Vector2f origin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
	sprite.setOrigin(origin);
	sf::Vector2f scale(size.x / texture.getSize().x, size.y / texture.getSize().y);
	sprite.setScale(scale);
	sprite.setPosition(position);
	target.draw(sprite);
}