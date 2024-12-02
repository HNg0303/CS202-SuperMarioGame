#include "Physics.h"

b2World Physics::world{(b2Vec2(0.0f, -9.8f))}; //intialize gravity in this world => 9.8m/s^2 down to Earth.

void Physics::Init() {

}

void Physics::Update(float deltaTime) {
	world.Step(deltaTime, 6, 2);
}