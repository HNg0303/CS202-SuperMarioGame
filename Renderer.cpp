#include "Renderer.h"

Renderer::Renderer(sf::RenderTarget& t) : target(t) {} //Target is Window

sf::RenderWindow* Renderer :: getRenderWindow() {
	return dynamic_cast<sf::RenderWindow*>(&target);
}

void Renderer::Draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size, float angle) {
	sprite.setTexture(texture, true);
	sf::Vector2f origin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
	sprite.setOrigin(origin);
	sf::Vector2f scale(size.x / texture.getSize().x, size.y / texture.getSize().y);
	sprite.setScale(scale);
	sprite.setPosition(position);
	sprite.setRotation(angle);
	target.draw(sprite);
}