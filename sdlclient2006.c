//compile:  gcc `sdl-config --cflags --libs` -o sdl2600 sdl2600.c gpio_driver.c

#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "sdl2600.h"
#include "packet2600.h"
#include "ctrl2600.h"
#include "gpio_driver.h"

#define PORT 2600

int network_setup( char* hostname, int port )
{
	struct hostent *he;
    struct sockaddr_in their_addr; // connector's address information 

    if ((he=gethostbyname(hostname)) == NULL) {  // get the host info 
        herror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    their_addr.sin_family = AF_INET;    // host byte order 
    their_addr.sin_port = htons(port);  // short, network byte order 
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof their_addr) == -1) {
        perror("connect");
        exit(1);
    }
    
    return sockfd;
}

char buildButtonMessage( enum button_ev_type be, enum button_type bt )
{
	return (bt << 3) | (be << 2);
}

char buildQuitMessage( )
{
	return 1;
}

void sendmsg( int fd, char msg )
{
	buf[0] = msg;
	if (send(fd, buf, 1, 0) == -1)
    	perror("send");
}

int main( int argc, char *argv[] )
{
	int sockfd, numbytes;
	char buf[1];
	
    if (argc != 2) {
		sockfd = network_setup( "localhost", PORT );
    } else {
	    sockfd = network_setup( argv[1], PORT );
	}

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
						sendmsg( sockfd, buildButtonMessage( BE_PRESS, BT_UP ) );
						break;
					case SDLK_DOWN:
						sendmsg( sockfd, buildButtonMessage( BE_PRESS, BT_DOWN ) );
						break;
					case SDLK_LEFT:
						sendmsg( sockfd, buildButtonMessage( BE_PRESS, BT_LEFT ) );
						break;
					case SDLK_RIGHT:
						sendmsg( sockfd, buildButtonMessage( BE_PRESS, BT_RIGHT ) );
						break;
					case SDLK_SPACE:
						sendmsg( sockfd, buildButtonMessage( BE_PRESS, BT_SPACE ) );
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
						sendmsg( sockfd, buildButtonMessage( BE_RELEASE, BT_UP ) );
						break;
					case SDLK_DOWN:
						sendmsg( sockfd, buildButtonMessage( BE_RELEASE, BT_DOWN ) );
						break;
					case SDLK_LEFT:
						sendmsg( sockfd, buildButtonMessage( BE_RELEASE, BT_LEFT ) );
						break;
					case SDLK_RIGHT:
						sendmsg( sockfd, buildButtonMessage( BE_RELEASE, BT_RIGHT ) );
						break;
					case SDLK_SPACE:
						sendmsg( sockfd, buildButtonMessage( BE_RELEASE, BT_SPACE ) );
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
	
	sendmsg( sockfd, buildQuitMessage( ) );
	
	close( sockfd );
	
	SDL_Quit();

	return 0;
}

