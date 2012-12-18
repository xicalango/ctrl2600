

#ifndef __PACKET_2600_H
#define __PACKET_2600_H

enum message_type
{
	MT_BUTTON = 0x00,
	MT_QUIT = 0x01
};

enum button_ev_type
{
	BE_PRESS = 0x0,
	BE_RELEASE = 0x1
};

enum button_type
{
	BT_UP = 0x00,
	BT_DOWN = 0x01,
	BT_LEFT = 0x02,
	BT_RIGHT = 0x03,
	BT_BUTTON = 0x04
};

#endif

