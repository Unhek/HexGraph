#pragma once

#include "Utility.h"

class Button
{
public:
	Button(uint8 InWidth, uint8 InHeight, int8 InX, int8 InY);
	~Button();

	uint8 Width;
	uint8 Height;
	
	int8 X;
	int8 Y;
};

