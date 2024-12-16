#pragma once
#include "INCLUDE.h"
#include <unordered_map>

class Resources
{
public:
	static unordered_map<string, Texture> textures;
	static unordered_map<string, SoundBuffer> sfx;
	void loadResource();
	Texture getTexture(string textureName);
};

string convertToUnixPath(const string& path);

