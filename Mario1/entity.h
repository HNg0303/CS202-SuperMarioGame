#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include <filesystem>
#include <vector>
#include <iostream>

using namespace sf;
class Entity // Drawable makes possible to use window.draw(object)
{
	public:
	
	std::string name;
	std::vector<sf::Texture> frames;
	int currentFrame;
	float frameDuration;
	sf::Clock clock;
	
	float direction; //left/right

	Vector2f position; //starting position


public:
	Entity(std::string name_i, float frameDuration_i, float x, float y):
	name(name_i), frameDuration(frameDuration_i), currentFrame(0), direction(1.0) 
	{
		position.x = x;
		position.y = y;
		frames = loadFrame("assets/frame/" + name);
		if (frames.empty())
		{
			std::cerr << "No frames loaded for entity " << name << std::endl;
			return;
		}
	};

	std::vector<sf::Texture> loadFrame(std::string folderPath);
	void draw(sf::RenderWindow& window);
	virtual void update() = 0; 
};


class Moveable : public Entity
{
private:
	float speed; //velocity
	std::pair<float, float> xBound; //fixed bound [start, end]
	float yPosition;
public:
	Moveable(std::string name_i, float frameDuration_i, float speed_i, float start, float end, float y) :
		Entity(name_i, frameDuration_i, start, y), yPosition(y), speed(speed_i)
	{
		xBound = std::make_pair(start, end);
	};

	
	void update() override;
	void checkAndChangeDirection();
	void move();

};

class Unmoveable : public Entity
{
private:

public:
	Unmoveable(std::string name_i, float frameDuration_i, float x, float y) :
		Entity(name_i, frameDuration_i, x, y) {}


	void update() override;	
};

