#include "Renderer.h"

Renderer::Renderer(sf::RenderTarget& t) : target(t) {} //Target is Window

void Renderer::Draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size, float angle, bool faceLeft) {
	sprite.setTexture(texture, true);
	sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
	if (faceLeft)
		sprite.setScale(-size.x / texture.getSize().x, size.y / texture.getSize().y);
	else
		sprite.setScale(size.x / texture.getSize().x, size.y / texture.getSize().y);
	sprite.setPosition(position);
	sprite.setRotation(angle);
	target.draw(sprite);
}