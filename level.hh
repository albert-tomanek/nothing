#ifndef __MVGAME_LEVEL_H__
#define __MVGAME_LEVEL_H__

#include <stdint.h>
#include "direction.hh"

#define lvl_check(S, M)	if (!(S)) { if (this->error != NULL) { free(this->error); }; this->error = strdup(M); goto error;}

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
	uint8_t checksum;

public:

	/* Level parameters. Don't change these from outside the class;				*
	 * it'll most likely cause the program to seg-fault, and spoil your day.	*/

	int   width, height;
	int   player_x, player_y;
	char *name;
	int   coins;
	bool  exits_open;	// Whether the exits are open or closed.

	uint8_t *contents;	// Array of the tiles in the level.
	char *error;		// NULL unless there is an error

	/* Methods */
	Level();
	~Level();

	void load(char *filename);
	void draw(int x, int y);	// Draws the level, the top left being at (x,y), using termbox.

	uint8_t tile_at(int x, int y);	// Returns the value of the tile at (x,y)
	uint8_t tile_by(int x, int y, enum Direction direction);

	int check();				// Checks the level data integrity via the checksum. Returns 1 on pass and 0 on fail.
};

#endif
