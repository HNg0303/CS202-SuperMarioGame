#include "Camera.h"

Camera::Camera(float zoomLevel): zoomLevel(zoomLevel){}

sf::View Camera::GetView(sf::Vector2u windowSize) {
	float aspect = (float)windowSize.x / (float)windowSize.y; //Ex: 16/9, 4/3 ...
	if (aspect < 1.0f)
		viewResolution = sf::Vector2f(zoomLevel, zoomLevel / aspect);
	else
		viewResolution = sf::Vector2f(zoomLevel * aspect, zoomLevel);
	sf::View view(position, viewResolution);
	return view;
}