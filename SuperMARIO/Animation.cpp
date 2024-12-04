#include "Animation.h"
#include "Resources.h"



void Animation::Update(float deltaTime)
{
	currentTime += deltaTime;
}

Texture Animation::getTexture()
{
	if (currentTime > endTime)
	{
		currentTime -= endTime;
	}
	for (auto& frame : frames)
	{
		if (currentTime < frame.endTime)
			return frame.texture;
	}
	return Resources::textures["run1.png"];
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
