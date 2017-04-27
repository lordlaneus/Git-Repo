#pragma once
#include <vector>
class Animation
{
public:
	std::vector<int> frames;
	std::vector<float>timing;
	bool loop = true;
	float duration = 1;

	void addFrame(int, float);
	int getFrame(float = -1);
	float length();

	
};