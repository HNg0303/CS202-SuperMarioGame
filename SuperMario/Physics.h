#pragma once
#include <box2d/b2_world.h>
class Physics {
private:
	static b2World world;
public:
	void Init();
	void Update(float deltaTime);

};