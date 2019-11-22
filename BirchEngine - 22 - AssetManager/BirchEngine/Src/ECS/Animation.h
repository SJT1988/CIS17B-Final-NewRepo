#pragma once

struct Animation
{
	int index;
	int frames;
	int delay;

	Animation() {}
	Animation(int i, int f, int s)
	{
		index = i;
		frames = f;
		delay = s;
	}
};