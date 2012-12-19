ctrl2600
========

An ATARI2600 controller emulator using raspberry pis GPIO running on raspbian.

It uses 5 GPIO ports to trigger 5 optocouplers. Those are used to simulate the ATARI 2600 controler.

GPIO ports used
---------------
See ctrl2600.h.

Currently the following assignment is in use:

```C
#define GPIO_BUTTON 17
#define GPIO_UP 18
#define GPIO_DOWN 23
#define GPIO_LEFT 24
#define GPIO_RIGHT 25
```

Programs
========

This repository includes three programs:

* *sdl2600*: This application is using SDL for event management (key down/key up) and runs completely on the r/pi.
* *server2600*: This program starts a simple TCP-server on port 2600 waiting for connections by a client. The component must run on the r/pi.
* *sdlclient2600*: This is the TCP-client for server using SDL for event management and can be run anywhere.

sdl2600
=======

Compilation
-----------

On the r/pi call:

	gcc `sdl-config --cflags --libs` -o sdl2600 sdl2600.c gpio_driver.c
	
Make sure you have the SDL development libraries installed.

Usage
-----

The application must be run as root (as it performs direct memory I/O).
It opens a blank 320 x 240 screen (which stays blank). You can now press one of the following keys to interact with the 2600:

* Arrow-Keys: directional input
* Space: fire button

server 2600
===========

Compilation
-----------

	gcc -Wall -o server2600 server2600.c gpio_driver.c
	
Usage
-----

Just start the server as root and it will listen on port 2600 for one incoming connection.

Protocol
--------

2 bits: Message Type
* 00 Button Event
* 01 Quit Event
* 10 Unused
* 11 Unused

In case of a Button Event:

1 bit: Pressed/Released Flag
* 0 Press
* 1 Release

3 bits: Button Type
* 000 Up
* 001 Down
* 010 Left
* 011 Right
* 100 Button

sdlclient2600
=============

Compilation
-----------

	gcc `sdl-config --cflags --libs` -o sdlclient2600 sdlclient2600.c
	
Usage
-----

First start the server on the r/pi then start the client with

	sdlclient2600 <<hostname>>

When omitting the hostname it will use 'localhost' as default.
It will work like *sdl2600*, but instead of using the GPIO directly it sends commands to the server.

References
==========

* http://elinux.org/RPi_Low-level_peripherals
* http://beej.us/guide/bgnet/