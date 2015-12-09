#include "KeyStack.h"

void KeyStack::add(SDL_Keycode key)
{
	remove(key);
	stack.push_back(key);
}
bool KeyStack::contains(SDL_Keycode key)
{
	for (int i = 0; i < this->stack.size(); i++)
	{
		if (stack[i] == key)
		{
			return true;
		}

	}
	return false;
}


int KeyStack::index(SDL_Keycode key)
{
	for (int i = 0; i < this->stack.size(); i++)
	{
		if (stack[i] == key)
		{
			return i;
		}

	}
	return -1;
}

void KeyStack::remove(SDL_Keycode key)
{
	if(contains(key))
	{
		stack.erase(stack.begin() + index(key));
	}
}
SDL_Keycode KeyStack::top()
{
	if (stack.empty())
		return NULL;
	return stack.back();
}
