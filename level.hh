#ifndef __MVGAME_LEVEL_H__
#define __MVGAME_LEVEL_H__

#include <stdint.h>

class Level
{
	int width, height;
	uint8_t *contents;
	
public:
	
	Level(char *filename);
	~Level();
	
	void draw(int x, int y);
}

#endif
