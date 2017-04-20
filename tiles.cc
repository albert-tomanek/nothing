#include <stdint.h>
#include <stdlib.h>
#include <termbox.h>

#include "tiles.hh"

void mv_tile_create(uint8_t type, struct tb_cell *cell, bool exit_open)
{
	if (cell == NULL)
		return;
	
	switch (type)
	{
	case MV_TILE_EMPTY:
	{
		cell->ch = ' ';
		cell->fg = TB_DEFAULT;
		cell->bg = TB_BLACK;
		break;
	}
	case MV_TILE_GROUND:
	{
		cell->ch = ' ';
		cell->fg = TB_DEFAULT;
		cell->bg = TB_BLUE;
		break;
	}
	case MV_TILE_WALL:
	{
		cell->ch = '#';
		cell->fg = TB_WHITE;
		cell->bg = TB_BLUE;
		break;
	}
	case MV_TILE_COIN:
	{
		cell->ch = '+';
		cell->fg = TB_GREEN | TB_BOLD;
		cell->bg = TB_BLUE;
		break;
	}
	case MV_TILE_EXIT:
	{
		cell->ch = (exit_open ? 'O' : 'X');
		cell->fg = (exit_open ? TB_GREEN : TB_RED) | TB_BOLD;
		cell->bg =  TB_BLUE;
		break;
	}

	default:
	{
		cell->ch = '@';
		cell->fg = TB_WHITE | TB_BOLD;
		cell->bg = TB_RED;
		break;
	}
	}
}
