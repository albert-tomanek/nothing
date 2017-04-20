
#ifndef __EDIT_MAIN_H__
#define __EDIT_MAIN_H__

#include "direction.hh"

class Cursor
{
	/* These are the x and y of the cursor IN THE LEVEL, 	*
	 * not on the screen.									*/

public:

	int x;
	int y;
	
	Cursor();
	void move(Direction direction);
};

#endif
