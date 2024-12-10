#include "entity.h"


std::vector<sf::Texture> Entity::loadFrame(std::string folderPath)
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
				std::cerr << "Successful to load : " << entry.path().string() << std::endl;
			}
			else
			{
				std::cerr << "Failed to load: " << entry.path().string() << std::endl;
			}
		}
	}

	return textures;
}

void Entity::draw(sf::RenderWindow& window)
{
	if (!frames.empty())
	{
		sf::Sprite sprite;
		sprite.setTexture(frames[currentFrame]);
		sprite.setScale(-1.f * direction, 1.f);
		sprite.setPosition(position);
		window.draw(sprite);
	}
}



void Moveable::update() 
{
	//std::cout << position.x << " " << position.y;
	if (clock.getElapsedTime().asSeconds() > frameDuration)
	{
		currentFrame = (currentFrame + 1) % frames.size();
		//std::cout << "check frame " << currentFrame << " " << frames.size() << '\n';
		clock.restart();
	}
	move();
}

void Moveable::move()
{
	position.x += speed * direction;
	checkAndChangeDirection();
}

void Moveable::checkAndChangeDirection()
{
	if (position.x <= xBound.first || position.x >= xBound.second)
	{
		direction = -direction; // Reverse direction
	}
}


void Unmoveable::update()
{
	//std::cout << "unmoveable" ;
	//std::cout << position.x << " " << position.y;
	if (clock.getElapsedTime().asSeconds() > frameDuration)
	{
		currentFrame = (currentFrame + 1) % frames.size();
		clock.restart();
		//std::cout << "check frame " << currentFrame << " " << frames.size() << '\n';
	}
}