#pragma once

#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include"entity.h"

class Goombas : public Moveable
{
private:

public:
	Goombas(float frameDuration_i, std::pair<float, float> bound_i) : Moveable(frameDuration_i, bound_i)
	{
		int n = 2;
		std::vector<size_t> currentFrames(11, 0);
		std::vector<std::vector<sf::Texture>> textures(11);
		/*for (int i = 0; i < 2; ++i)
		{
			textures[i] = loadFrame("assets/frame/" + "Goombas");
			if (textures[i].empty())
			{
				std::cerr << "No frames loaded for entity " << i << std::endl;
				return;
			}
		}*/
	}

};