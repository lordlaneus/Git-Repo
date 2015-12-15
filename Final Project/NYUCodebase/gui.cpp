#include "GUI.h"

using namespace std;

void GUI::update(float elapsed)
{
	if (showTime != -1 && showTime > 0)
	{
		showTime -= elapsed;
		if (showTime <= 0)
		{
			visible = false;
		}
	}
	
	
}
void GUI::show(float duration)
{
	showTime = duration;
	visible = true;
}