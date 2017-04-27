#pragma once
#include <vector>

#include <SDL.h>
class KeyStack{

public:
	std::vector<SDL_Keycode> stack;

	void add(SDL_Keycode);
	bool contains(SDL_Keycode);
	int index(SDL_Keycode);
	void remove(SDL_Keycode);
	SDL_Keycode top();
};