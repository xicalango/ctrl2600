//compile:  gcc `sdl-config --cflags --libs` -o sdl2600 sdl2600.c gpio_driver.c

#include <SDL.h>
#include <stdio.h>
#include "sdl2600.h"
#include "ctrl2600.h"
#include "gpio_driver.h"



int main( void )
{
	//SDL_Surface* screen = NULL;
	SDL_Event event;
	int running = 0;

	gpio_setup();
	
	USE_GPIO( GPIO_BUTTON );
	USE_GPIO( GPIO_UP );
	USE_GPIO( GPIO_DOWN );
	USE_GPIO( GPIO_LEFT );
	USE_GPIO( GPIO_RIGHT );

	SDL_Init( SDL_INIT_VIDEO );
	
	SDL_SetVideoMode( 320, 240, 16, SDL_SWSURFACE );
	
	running = !0;
	
	while( running )
	{
		while( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_KEYDOWN )
			{
				switch( event.key.keysym.sym )
				{
					case SDLK_UP:
						GPIO_SETX(GPIO_UP);
						break;
					case SDLK_DOWN:
						GPIO_SETX(GPIO_DOWN);
						break;
					case SDLK_LEFT:
						GPIO_SETX(GPIO_LEFT);
						break;
					case SDLK_RIGHT:
						GPIO_SETX(GPIO_RIGHT);
						break;
					case SDLK_SPACE:
						GPIO_SETX(GPIO_BUTTON);
						break;
					case SDLK_q:
						running = 0;
						break;
					default:
						break;
				}
			}
			else if( event.type == SDL_KEYUP )
			{
				switch( event.key.keysym.sym )
				{
					case SDLK_UP:
						GPIO_CLRX(GPIO_UP);
						break;
					case SDLK_DOWN:
						GPIO_CLRX(GPIO_DOWN);
						break;
					case SDLK_LEFT:
						GPIO_CLRX(GPIO_LEFT);
						break;
					case SDLK_RIGHT:
						GPIO_CLRX(GPIO_RIGHT);
						break;
					case SDLK_SPACE:
						GPIO_CLRX(GPIO_BUTTON);
						break;
					default:
						break;
				}
			}
			else if( event.type == SDL_QUIT ) 
			{ 
				running = 0;
			} 
		}
	}
	
	
	
	SDL_Quit();

	return 0;
}

