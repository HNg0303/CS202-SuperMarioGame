#pragma once
#include "INCLUDE.h"
#include "Physics.h"

using namespace sf;

extern vector<Entity*> onEntities; //For using on-screen entities
void deleteEntity(Entity* entity);
void clearEntities();


//Composite Design Pattern
class Entity 
{
public:

	string name;
	vector<Texture> frames;
	int currentFrame;
	double frameDuration;
	Clock clock;
	b2Body* body = nullptr;
	FixtureData* fixtureData = nullptr;

	bool faceLeft = false;
	bool deleted = false;

	Vector2f coords; //Coordinates in grid.
	Vector2f size; //Render size
	Vector2f position; //starting position
	float x_direction = 1.0f; //left/right
	float y_direction = -1.0f;
	//Method
	Entity(string name_i, double frameDuration_i, float x, float y, Vector2f coords);
	vector<Texture> loadFrame(string folderPath);
	void draw(RenderWindow* window, const Vector2f& size);
	virtual void Begin() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual string getName();
	virtual ~Entity() = default;
	Vector2f getCoords();
	void markDeleted();
};


class Moveable : public Entity
{
private:
	float speed; //velocity
	pair<float, float> xBound; //fixed bound [start, end]
	pair<float, float> yBound;
	float yPosition; // what does this even do
public:
	Moveable(string name_i, double frameDuration_i, float speed_i, float x_start, float x_end, float y_start, float y_end, Vector2f coords) :
		Entity(name_i, frameDuration_i, x_start, y_start, coords), speed(speed_i)
	{
		yPosition = position.y;
		yBound = make_pair(y_start, y_end);
		xBound = make_pair(x_start, x_end);
		if (x_start == x_end)
			x_direction = 0;
		if (y_start == y_end)
			y_direction = 0;
	};
	float destroyingTimer = 0.0f;
	bool isDead = false;
	void Begin() override {};
	void Update(float deltaTime) override;
	void checkAndChangeDirection();
	void move();

};

class Unmoveable : public Entity
{
public:
	Unmoveable(string name_i, double frameDuration_i, float x, float y, Vector2f coords) :
		Entity(name_i, frameDuration_i, x, y, coords) {
	}
	void Begin() override {};
	void Update(float deltaTime) override;
};

class Coin : public Unmoveable {
public:
	Coin(string name_i, double frameDuration_i, float x, float y, Vector2f size, Vector2f coords) :
		Unmoveable(name_i, frameDuration_i, x, y, coords) {
		this->size = size;
	}
	void Begin() override;
	~Coin();
};

class Block : public Unmoveable {
public:
	Block(string name_i, double frameDuration_i, float x, float y, Vector2f size, Vector2f coords) :
		Unmoveable(name_i, frameDuration_i, x, y, coords) {
		this->size = size;
	}
	void Begin() override;
	~Block();
};

class QBlock : public Unmoveable {
	QBlock(string name_i, double frameDuration_i, float x, float y, Vector2f size, Vector2f coords) :
		Unmoveable(name_i, frameDuration_i, x, y, coords) {
		this->size = size;
	}
	//void Begin() override;
	//~QBlock();
};

class PowerUp : public Unmoveable {
public:
	PowerUp(string name_i, double frameDuration_i, float x, float y, Vector2f size, Vector2f coords) :
		Unmoveable(name_i, frameDuration_i, x, y, coords) {
		this->size = size;
	}
	void Begin() override;
	//void Update(float deltaTime) override;
	~PowerUp();
};

class Enemy : public Moveable, public ContactListener {
public:
	b2Fixture* fixture = nullptr;

	Enemy(string name_i, double frameDuration_i, float speed_i, float x_start, float x_end, float y_start, float y_end, Vector2f size, Vector2f coords) :
		Moveable(name_i, frameDuration_i, speed_i, x_start, x_end, y_start, x_end, coords) {
		this->size = size;
	}
	void Begin() override;
	void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
	void OnEndContact(b2Fixture* self, b2Fixture* other) override {};
	void Die();
	~Enemy();
};

class Flame : public Moveable, public ContactListener {
public:
	b2Fixture* fixture = nullptr;

	Flame(string name_i, double frameDuration_i, float speed_i, float x_start, float x_end, float y_start, float y_end, Vector2f size, Vector2f coords) :
		Moveable(name_i, frameDuration_i, speed_i, x_start, x_end, y_start, y_end, coords) {
		this->size = size;
	}

	void Begin() override;
	void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
	void OnEndContact(b2Fixture* self, b2Fixture* other) override {};

	~Flame();
};

class Elevator : public Moveable {
public:
	Elevator(string name_i, double frameDuration_i, float speed_i, float x_start, float x_end, float y_start, float y_end, Vector2f size, Vector2f coords) :
		Moveable(name_i, frameDuration_i, speed_i, x_start, x_end, y_start, y_end, coords)
	{
		this->size = size;
	};
	void Begin() override;


	~Elevator();
};

class Bowser : public Enemy {
public:
	Bowser(string name_i, double frameDuration_i, float speed_i, float x_start, float x_end, float y_start, float y_end, Vector2f size, Vector2f coords) :
		Enemy(name_i, frameDuration_i, speed_i, x_start, x_end, y_start, y_end, size, coords) { }
	float fireTime = 0.0f;
	void Update(float deltaTime) override;

	~Bowser();
};

