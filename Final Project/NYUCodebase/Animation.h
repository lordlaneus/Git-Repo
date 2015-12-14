#pragma once
#include <vector>
class Animation
{
public:
	std::vector<int> frames;
	std::vector<float>timing;


	void addFrame(int, float);
	int getFrame(float);
	float length();

	
};