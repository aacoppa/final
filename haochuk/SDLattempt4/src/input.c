#include "input.h"

void getInput()
{
	SDL_Event event;
	
	/* Loop through waiting messages and process them */
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		  /*
		  ** Closing the Window or pressing Escape will exit the program
		  ** Pressing Space will play the sound effect
		  */
		  
		case SDL_QUIT:
		  exit(0);
		  break;
		  
		case SDL_KEYDOWN:
		  switch (event.key.keysym.sym)
		    {
		    case SDLK_w:
		      input.up = 1;
		      break;
                      
		    case SDLK_s:
		      input.down = 1;
		      break;
                      
		    case SDLK_a:
		      input.left = 1;
		      break;
                      
		    case SDLK_d:
		      input.right = 1;
		      break;
                      
		    case SDLK_ESCAPE:
		      exit(0);
		      break;
                      
		    default:
		      break;
		    }
		  break;
		  
		case SDL_KEYUP:
		  switch (event.key.keysym.sym)
		    {
		    case SDLK_w:
		      input.up = 0;
		      break;
                      
		    case SDLK_s:
		      input.down = 0;
		      break;
                      
		    case SDLK_a:
		      input.left = 0;
		      break;
                      
		    case SDLK_d:
		      input.right = 0;
		      break;
                      
		    default:
		      break;
		    }
		  break;
		  
		  
		}
	}
}
