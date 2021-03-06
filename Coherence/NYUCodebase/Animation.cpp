#include "Animation.h"
#include <SDL.h>

using namespace std;

void Animation::addFrame(int frame, float length)
{
	frames.push_back(frame);
	timing.push_back(length);
}
int Animation::getFrame(float time)
{
	if (time == -1)
	{
		time = (float)SDL_GetTicks() / 1000;
	}
	if (loop)
	{
		time = fmod(time/duration, 1);
	}
	time *= length();
	for (int i = 0;i < timing.size(); i++)
	{
		time-= timing[i];
		if (time <= 0)
		{
			return frames[i];
		}
	}
	return frames.back();
}
float Animation::length()
{
	float totalLength = 0;
	for (int i = 0; i < timing.size(); i++)
	{
		totalLength += timing[i];
	}
	return totalLength;
}
