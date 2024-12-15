#pragma once
#include "INCLUDE.h"
#include <unordered_map>

class Resources
{
public:
	void loadResource();
	Texture getTexture(string textureName);
	static unordered_map<string, class Texture> textures;
	static unordered_map<string, class SoundBuffer> sfx;
};

string convertToUnixPath(const string& path);

