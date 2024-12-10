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
	
	float speed;
	float direction;

	Vector2f position;


	/*
	move: starting pos
	range [x1,x2]
	*/
	//void draw(RenderTarget& target, RenderStates state) const; // to use window.draw(object)
	
	/*Texture texture;
	Sprite sprite;
	float Width;
	float Height;
	float Velocity;

	Vector2f position;
	Vector2f velocity{ Velocity, Velocity };


	bool destroyMode = 0;
	bool isFriendly = 0;
	bool killable = true;


	sf::VertexArray m_vertices;
	sf::Texture m_texture;

	std::string file;*/

public:
	Entity(std::string name_i, float frameDuration_i, float speed_i):
	name(name_i), frameDuration(frameDuration_i), currentFrame(0), speed(speed_i), direction(1.0) {};

	std::vector<sf::Texture> loadFrame(std::string folderPath)
	{
		std::vector<sf::Texture> textures;

		for (const auto& entry : std::filesystem::directory_iterator(folderPath))
		{
			if (entry.is_regular_file())
			{
				sf::Texture texture;
				if (texture.loadFromFile(entry.path().string()))
				{
					textures.push_back(texture);
				}
				else
				{
					std::cerr << "Failed to load: " << entry.path().string() << std::endl;
				}
			}
		}

		return textures;
	}

	void draw(sf::RenderWindow& window)
	{
		
		if (!frames.empty())
		{
			sf::Sprite sprite;
			sprite.setTexture(frames[currentFrame]);
			sprite.setScale(-1.f*direction, 1.f);
			sprite.setPosition(position);
			window.draw(sprite);
		}
	}

	virtual void update() =0 ; 

	/*Vector2f getPosition();

	void setPosition(Vector2f position);


	void update();

	float left();
	float right();
	float top();
	float bottom();

	void moveLeft();
	void moveRight();
	void moveTop();
	void moveBottom();

	bool isAlive = true;*/

	/*void MovingDirectionLeft() { velocity.x = -Velocity; }
	void MovingDirectionRight() { velocity.x = Velocity; }

	Sprite getSprite() { return sprite; }

	void dead() { isAlive = false; }
	void deadAtOnce() { isAlive = false; }
	bool getIsAlive() { return isAlive; }
	bool getDestroyMode() { return destroyMode; }
	void reset();
	bool getIsFriendly() { return isFriendly; }

	void repair();
	bool isKillable() { return killable; }*/

	//void loadFrame(std::string folderPath, sf::RenderWindow& window);

};


class Moveable : public Entity
{
private:
	std::pair<float, float> xBound; //fixed bound [start, end]
	float yPosition;
public:
	Moveable(std::string name_i, float frameDuration_i, float speed_i, float start, float end, float y) : Entity(name_i, frameDuration_i, speed_i), yPosition(y)
	{
		xBound = std::make_pair(start, end);
		position.x = start;
		position.y = y;

		frames = loadFrame("assets/frame/" + name);
		if (frames.empty())
		{
			std::cerr << "No frames loaded for entity " << name << std::endl;
			return;
		}
	};

	
	void update() override
	{
		std::cout << position.x << " " << position.y;
		if (clock.getElapsedTime().asSeconds() > frameDuration)
		{
			currentFrame = (currentFrame + 1) % frames.size();
			std::cout << "check frame " << currentFrame << " " << frames.size() << '\n';
			clock.restart();
		}
		move();
	}

	void checkAndChangeDirection()
	{
		if (position.x <= xBound.first || position.x >= xBound.second)
		{
			direction = -direction; // Reverse direction
		}
	}

	void move()
	{
		position.x += speed * direction;
		checkAndChangeDirection();
	}

};