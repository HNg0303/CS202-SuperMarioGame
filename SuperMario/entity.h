#pragma once
#include "INCLUDE.h"
#include "Physics.h"

using namespace sf;

extern vector<Entity*> onEntities; //For using on-screen entities
void deleteEntity(Entity* entity);
void clearEntities();

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
	Vector2f coords; //Coordinates in grid.
	Vector2f size; //Render size
	Vector2f position; //starting position

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
	float yPosition;
	
public:
	Moveable(string name_i, double frameDuration_i, float speed_i, float start, float end, float y, Vector2f coords) :
		Entity(name_i, frameDuration_i, start, y, coords), yPosition(y), speed(speed_i)
	{
		xBound = make_pair(start, end);
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
		Entity(name_i, frameDuration_i, x, y, coords) {}
	void Begin() override {};
	void Update(float deltaTime) override;	
};

class Coin : public Unmoveable {
public: 
	Coin(string name_i, double frameDuration_i, float x, float y, Vector2f size, Vector2f coords):
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

class PowerUp : public Unmoveable {
public:
	PowerUp(string name_i, double frameDuration_i, float x, float y, Vector2f size, Vector2f coords):
		Unmoveable(name_i, frameDuration_i, x, y, coords) {
		this->size = size;
	}
	void Begin() override;
	~PowerUp();
};

class Enemy : public Moveable {
public:
	
	Enemy(string name_i, double frameDuration_i, float speed_i, float start, float end, float y, Vector2f size, Vector2f coords) :
		Moveable(name_i, frameDuration_i, speed_i, start, end, y, coords) {
		this->size = size;
	}
	void Begin() override;
	void Die();
	~Enemy();
};

class Flame : public Moveable, public ContactListener {
public:
	Flame(string name_i, double frameDuration_i, float speed_i, float start, float end, float y, Vector2f size, Vector2f coords) :
		Moveable(name_i, frameDuration_i, speed_i, start, end, y, coords) {
		this->size = size;
	}
	b2Fixture* fixture;
	void Begin() override;
	void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
	void OnEndContact(b2Fixture* self, b2Fixture* other) override {};

	~Flame();
};


