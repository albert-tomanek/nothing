#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <termbox.h>

#include "level.hh"

#include "tiles.h"
#include "error.hh"

Level :: Level(char *filename)
{
	/* We have to pre-declare these, because unlike C,								*
	 * C++ complains that the variable may not be initialised if you use a goto.	*/
	struct mvlvl_header *header;
	bool ismvfile;
	
	FILE *lvlfile = fopen(filename, "r");
	lvl_check(lvlfile, strerror(errno));	// Check for errors
	
	/* Load the header */
	header = malloc(sizeof(struct mvlvl_header));
	fread(header, sizeof(struct mvlvl_header), 1, lvlfile);
	
	/* Ensure the file is a .mv file */
	ismvfile = true;
	ismvfile = (header->magic_number[0] == 0xA6);
	ismvfile = (header->magic_number[1] == 0xC4);
	ismvfile = (! strncmp(header->magic_string, "MVGAME", 6));
	ismvfile = (header->padding1 == 0x00);
	ismvfile = (header->padding2 == 0x00);
	
	lvl_check(ismvfile == true, "File is not a mvgame level.")
	
	/* Ensure it is of the correct format version */
	lvl_check(header->fmtver == 0x01, "Format version not supported.");
	
	/* Keep a copy of the level variables */
	this->name     = strndup(header->lvl_name, 16);
	this->width    = (int) header->lvl_width;
	this->height   = (int) header->lvl_height;
	this->player_x = (int) header->player_x;
	this->player_y = (int) header->player_y;
	
	/* Load the level data */
	this->contents = malloc(this->width * this->height * sizeof(uint8_t));
	
	uint8_t byte;
	int index;
	
	while (index < this->width * this->height)
	{
		byte = (uint8_t) fgetc(lvlfile);	// If there *was* a read error, it would most likley be found by the checksum anyway
		
		/* First tile */
		this->contents[index] = (byte & 0xF0) >> 4;
		index++;
		
		/* Second tile */
		this->contents[index] = (byte & 0x0F);
		index++;
	}
	
	/* Read the checksum */
	fseek(lvlfile, -1, SEEK_END);		// We should already be 1 byte before the end, but we fseek anyway to skip any padding in case the level contents are and odd number of nibbles long.
	this->checksum = fgetc(lvlfile);
	
	fclose(lvlfile);
	
	return;
	
error:
	this->width  = 0;
	this->height = 0;
	this->contents = NULL;
	this->name     = NULL;
	
	if (lvlfile) fclose(lvlfile);
	
	return;
}

Level :: ~Level()
{
	free(this->contents);
	free(this->name);
	free(this->error);
}

inline uint8_t Level :: tile_at(int x, int y)
{
	return this->contents[(y * this->height) + x];
}

void Level :: draw(int tl_x, int tl_y)
{
	for (int y = 0; y < this->height && tl_y + y < tb_height(); y++)
	{
		for (int x = 0; x < this->width && tl_x + x < tb_width(); x++)
		{
			uint8_t tile = this->tile_at(x, y);
			
			switch (tile)
			{
			case MV_TILE_EMPTY:
				tb_change_cell(x, y, ' ', TB_DEFAULT, TB_BLACK);
				break;
			case MV_TILE_GROUND:
				tb_change_cell(x, y, ' ', TB_DEFAULT, TB_BLUE);
				break;
			case MV_TILE_WALL:
				tb_change_cell(x, y, '#', TB_WHITE, TB_BLUE);
				break;
			case MV_TILE_COIN:
				tb_change_cell(x, y, '+', TB_GREEN | TB_BOLD, TB_BLUE);
				break;

			default:
				tb_change_cell(x, y, '@', TB_WHITE | TB_BOLD, TB_RED);
				break;
			}
			
		}
	}
	
}

int Level :: coins()
{
	/* Not implemented yet */
	
	return 0;
}

int Level :: check()
{
	uint32_t sum = 0;
	
	for (int i = 0; i < this->width * this->height; i++)
	{
		sum += contents[i];
	}
		
	return (this->checksum == sum % 256);
}
