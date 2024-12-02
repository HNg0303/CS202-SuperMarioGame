#pragma once
#include <box2d/b2_world.h>
class Physics {
	//b2_world is a world that controls every bodies in this world.
public:
	void Init();
	void Update(float deltaTime);
	static b2World world; //Static object helps this class act as a mediator to interaction between Mario and Map. 
};