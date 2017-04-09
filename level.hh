#ifndef __MVGAME_LEVEL_H__
#define __MVGAME_LEVEL_H__

#include <stdio.h>
#include <stdint.h>

#define FORMAT_VERSION 0x01

#define lvl_check(S, M)	if (!S) {this->error = strdup(M); goto error;}

struct mvlvl_header
{
	uint8_t magic_number[2];
	char    magic_string[6];
	uint8_t fmtver;
	uint8_t padding1;
	
	uint8_t lvl_width;
	uint8_t lvl_height;
	uint8_t player_x;
	uint8_t player_y;
	char    lvl_name[16];
	uint8_t padding2;
};

class Level
{
public:
	
	/* Level parameters. Don't change these from outside the class;				*
	 * it'll most likely cause the program to seg-fault, and spoil your day.	*/
	 
	int width, height;	
	int player_x, player_y;
	char *name;

	uint8_t *contents;	// Array of the tiles in the level.
	char *error;		// NULL unless there is an error
	
	/* Methods */
	Level(char *filename);
	~Level();
	
	void draw(int x, int y);	// Draws the level, the top left being at (x,y), using termbox.
	
	int coins();				// Finds how many coins there are in the level, so that YOU don't have to.
};

uint8_t mvlvl_format_version(FILE* file);
uint8_t mvlvl_check_format(FILE* file);

#endif
