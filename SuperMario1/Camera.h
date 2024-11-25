#pragma once
#include <SFML/Graphics.hpp>

class Camera {
public:
	Camera(float zoomLevel = 0.5f);
	sf::View GetView(sf::Vector2u windowSize);
	float zoomLevel;
	sf::Vector2f position;
};