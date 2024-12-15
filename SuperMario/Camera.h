#pragma once
#include <SFML/Graphics.hpp>

class Camera {
private:
	sf::Vector2f viewResolution;
public:
	Camera(float zoomLevel = 0.5f);
	sf::Vector2f getViewRes() { return viewResolution; }
	sf::View GetView(sf::Vector2u windowSize);
	float zoomLevel;
	sf::Vector2f position;
};