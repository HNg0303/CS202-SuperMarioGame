#pragma once
#include <box2d/b2_world.h>
class Physics {
private:
	static b2World world; //Static object helps this class act as a mediator to interaction between Mario and Map.
public:
	void Init();
	void Update(float deltaTime);

};