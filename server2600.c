/*
** server.c -- a stream socket server demo
* taken from http://beej.us/guide/bgnet/examples/server.c
* modifyed by <weldale@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "packet2600.h"

#define MYPORT 2600

#define BACKLOG 1     // how many pending connections queue will hold



int main(void)
{
    int sockfd, new_fd, numbytes;  // listen on sock_fd, new connection on new_fd
    char buf[1];
    struct sockaddr_in my_addr;    // my address information
    struct sockaddr_in their_addr; // connector's address information
    socklen_t sin_size;
    int yes=1;
    
    int running = 1;
    

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    
    my_addr.sin_family = AF_INET;         // host byte order
    my_addr.sin_port = htons(MYPORT);     // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sin_size = sizeof their_addr;
    if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
        perror("accept");
        exit(1);
    }
    
    printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
    
    while(running)
    {
    	int msg_type;
    
    	if((numbytes = recv( new_fd, buf, 1, 0)) == -1) {
    		perror("recv");
    		exit(1);
    	}
    	
    	if(numbytes == 0)
    	{
    		continue;
    	}
    	
    	msg_type = buf[0] & 0x03; // 0b00000011
    	
    	switch( msg_type )
    	{
    	case MT_BUTTON: {
			int button_event, button;
			
			button_event = (buf[0] & (1 << 2)) >> 2; // 0b00000100
			button = (buf[0] & (7 << 3)) >> 3; // 0b00111000
			
			switch( button_event )
			{
				case BE_PRESS:
					
					switch( button )
					{
						case BT_UP:
							printf("UP pressed\n");
							break;
						case BT_DOWN:
							printf("DOWN pressed\n");
							break;
						case BT_LEFT:
							printf("LEFT pressed\n");
							break;
						case BT_RIGHT:
							printf("RIGHT pressed\n");
							break;
						case BT_BUTTON:
							printf("BUTTON pressed\n");
							break;
						default:
							printf("%d pressed\n", button );
							break;
					}
					
					break;
				case BE_RELEASE:

					switch( button )
					{
						case BT_UP:
							printf("UP released\n");
							break;
						case BT_DOWN:
							printf("DOWN released\n");
							break;
						case BT_LEFT:
							printf("LEFT released\n");
							break;
						case BT_RIGHT:
							printf("RIGHT released\n");
							break;
						case BT_BUTTON:
							printf("BUTTON released\n");
							break;
						default:
							printf("%d pressed\n", button );
							break;
					}

					break;
			}
    		
    		
    	
    	} break;
    	
    	case MT_QUIT: {
    		running = 0;
    		continue;
    	} break;
    	
    	default: {
    		printf("%d message type\n", msg_type);
    	}
    	
    	}
    	
    }
    

	printf("server: shutting down...\n");

    close(new_fd);  // parent doesn't need this
    close(sockfd);

    return 0;
}

