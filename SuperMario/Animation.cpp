#include "Animation.h"
#include "Resources.h"



void Animation::Update(float deltaTime)
{
	currentTime += deltaTime;
}

Texture Animation::getTexture()
{
	if (currentTime >= endTime)
	{
		currentTime = 0.0f;
		currentFrame = 0;
	}
	if (currentTime >= frames[currentFrame].endTime)
		currentFrame++;
	return frames[currentFrame].texture;
}

Animation& Animation::operator=(const Animation& other)
{
	currentTime = other.currentTime;
	endTime = other.endTime;
	while (!frames.empty()) frames.pop_back();
	for (int i = 0; i < other.frames.size(); ++i)
		frames.push_back(other.frames[i]);
	return *this;
}
