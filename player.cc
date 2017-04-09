#include <termbox.h>
#include "player.hh"

Player :: Player()
{
	this->symbol = '*';
}

void Player :: move(enum Direction direction)
{
	/* Changes the player's location*/
	switch (direction)
	{
		case NORTH:
			this->y -= 1;
		break;
		
		case SOUTH:
			this->y += 1;
		break;
		
		case EAST:
			this->x += 1;
		break;
		
		case WEST:
			this->x -= 1;
		break;
	}
}

void Player :: draw()
{
	tb_change_cell(this->x, this->y, (uint32_t) this->symbol, TB_GREEN, TB_BLUE);
}

void Player :: draw_xy(int x, int y)
{
	tb_change_cell(x, y, (uint32_t) this->symbol, TB_GREEN, TB_BLUE);
}
