#pragma once
#include "INCLUDE.h"
#include "Physics.h"

using namespace sf;
class Entity // Drawable makes possible to use window.draw(object)
{
public:
	
	string name;
	vector<Texture> frames;
	int currentFrame;
	double frameDuration;
	Clock clock;
	b2Body* body = nullptr;
	FixtureData* fixtureData = nullptr;

	bool deleted = false;
	float direction; //left/right
	Vector2f size; //Render size
	Vector2f position; //starting position

	//Method
	Entity(string name_i, double frameDuration_i, float x, float y);
	vector<Texture> loadFrame(string folderPath);
	void draw(RenderWindow* window, const Vector2f& size);
	virtual void Begin() = 0;
	virtual void update() = 0; 
	virtual string getName();
	virtual ~Entity() = default;
	void markDeleted();
};


class Moveable : public Entity
{
private:
	float speed; //velocity
	pair<float, float> xBound; //fixed bound [start, end]
	float yPosition;
public:
	Moveable(string name_i, double frameDuration_i, float speed_i, float start, float end, float y) :
		Entity(name_i, frameDuration_i, start, y), yPosition(y), speed(speed_i)
	{
		xBound = make_pair(start, end);
	};

	void Begin() override {};
	void update() override;
	void checkAndChangeDirection();
	void move();

};

class Unmoveable : public Entity
{
public:
	Unmoveable(string name_i, double frameDuration_i, float x, float y) :
		Entity(name_i, frameDuration_i, x, y) {}
	void Begin() override {};
	void update() override;	
};

class Coin : public Unmoveable {
public: 
	Coin(string name_i, double frameDuration_i, float x, float y, Vector2f size):
		Unmoveable(name_i, frameDuration_i, x, y) {
		this->size = size;
	}
	void Begin() override;
	~Coin();
};

class Block : public Unmoveable {
public:
	Block(string name_i, double frameDuration_i, float x, float y, Vector2f size) :
		Unmoveable(name_i, frameDuration_i, x, y) {
		this->size = size;
	}
	void Begin() override;
	~Block();
};

class Enemy : public Moveable {
public:
	Enemy(string name_i, double frameDuration_i, float speed_i, float start, float end, float y, Vector2f size) :
		Moveable(name_i, frameDuration_i, speed_i, start, end, y) {
		this->size = size;
	}
	void Begin() override;
};