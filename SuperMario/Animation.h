#pragma once
#include "INCLUDE.h"

struct Frame //to get the frames of the animation
{
	Frame(float time = 0.0f, Texture _texture = Texture()) : endTime(time), texture(_texture) { }
	float endTime; 
	Texture texture;
};

class Animation
{
public:
	Animation(float time = 0.0f, vector<Frame> _frames = { }) : endTime(time), frames(_frames) { } //initialize the Animation
	void Update(float deltaTime); //to switch between the frames in vector<Frame> frames
	Texture getTexture(); //to get the Texture of the current Frame
	Animation& operator=(const Animation& other);
private:
	float currentTime = 0.0f;
	float endTime;
	vector<Frame> frames;
};