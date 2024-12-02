#pragma once
#include <SFML/Graphics.hpp>
#include "INCLUDE.h"
#include <unordered_map>

class Resources
{
public:
	std::unordered_map<std::string, sf::Texture> textures;
	void loadResource();
	Texture getTexture(string textureName);
};

string convertToUnixPath(const string& path);

