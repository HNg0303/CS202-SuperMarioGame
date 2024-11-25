#pragma once
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Resources.h"
class Mario
{
private:
public:
	const float movementSpeed = 0.3f;
	float height = 0.04f;
	float width = 0.015f;
	sf::Vector2f position{};
	float angle{};
	void Update(float deltaTime);
	void Draw(Renderer& renderer);
};

