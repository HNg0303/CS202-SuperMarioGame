#pragma once
#include <SFML/Graphics.hpp>
#include "INCLUDE.h"
#include <unordered_map>

class Resources
{
public:
	void loadResource();
	Texture getTexture(string textureName);
	static unordered_map<string,class Texture> textures;
};

string convertToUnixPath(const string& path);

