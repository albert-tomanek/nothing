#ifndef __EDIT_BBAR_H__
#define __EDIT_BBAR_H__

#include <stdint.h>

class BottomBar
{
	/* The bar at the bottom of the screen */

	char *message;	// Stores the message currently being shown.

public:
	uint16_t bg;

	BottomBar();
	~BottomBar();

	void draw();
	void display(char *message);
	void query(char *message, char *buffer, size_t buffer_size);
};

#endif
