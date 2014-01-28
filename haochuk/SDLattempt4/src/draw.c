#include "draw.h"

extern void drawMap(void);
extern void drawPlayer(void);
extern void drawEntities(void);

void draw()
{
	/* Draw the map */
	
	drawMap();

	/* Draw the player */

	drawPlayer();

	/* Swap the buffers */

	SDL_Flip(screen);
	
	/* Sleep briefly */
	
	SDL_Delay(1);
}

void delay(unsigned int frameLimit)
{
	unsigned int ticks = SDL_GetTicks();

	if (frameLimit < ticks)
	{
		return;
	}
	
	if (frameLimit > ticks + 16)
	{
		SDL_Delay(16);
	}
	
	else
	{
		SDL_Delay(frameLimit - ticks);
	}
}
