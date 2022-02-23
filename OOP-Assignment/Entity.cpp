#include "Entity.h"

// This function returns the integer version of the floating rect passed in
SDL_Rect FRect::toRect() const
{
	int intX = static_cast<int>(this->x);
	int intY = static_cast<int>(this->y);
	int intW = static_cast<int>(this->w);
	int intH = static_cast<int>(this->h);

	return { intX,intY,intW,intH };
}