#include "Camera.h"

Camera::Camera(float zoomLevel): zoomLevel(zoomLevel){}

sf::View Camera::GetView(sf::Vector2u windowSize) {
	float aspect = (float)windowSize.x / (float)windowSize.y; //Ex: 16/9, 4/3 ...
	sf::Vector2f size;
	if (aspect < 1.0f)
		size = sf::Vector2f(zoomLevel, zoomLevel / aspect);
	else
		size = sf::Vector2f(zoomLevel * aspect, zoomLevel);
	sf::View view(position, size);
	return view;
}