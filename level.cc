#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <termbox.h>

#include "level.hh"

#include "tiles.hh"
#include "error.hh"

Level :: Level()
{
	this->exits_open = false;

	this->width  = 0;
	this->height = 0;

	this->player_x = 0;
	this->player_y = 0;

	this->contents = NULL;
	memset(this->name, '\0', LVL_NAMELEN + 1);

	this->error = NULL;

	return;
}

Level :: ~Level()
{
	free(this->contents);
	free(this->error);
}

void Level :: load(char *filename)
{
	/* We have to pre-declare these, because unlike C,								*
	 * C++ complains that the variable may not be initialised if you use a goto.	*/
	struct mvlvl_header *header;
	bool ismvfile;
	FILE *lvlfile;

	lvl_check(filename, "Null-pointer passed to Level::load for 'char *filename'.");

	lvlfile = fopen(filename, "r");
	lvl_check(lvlfile, strerror(errno));	// Check for errors

	/* Load the header */
	header = (struct mvlvl_header *) malloc(sizeof(struct mvlvl_header));
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
	strncpy(this->name, header->lvl_name, LVL_NAMELEN);
	this->width    = (int) header->lvl_width;
	this->height   = (int) header->lvl_height;
	this->player_x = (int) header->player_x;
	this->player_y = (int) header->player_y;

	free(header);

	/* Load the level data */
	this->contents = (uint8_t *) malloc(this->width * this->height * sizeof(uint8_t));

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
	this->checksum = (uint8_t) fgetc(lvlfile);

	/* Count the coins */
	this->coins = 0;

	for (int i = 0; i < this->width * this->height; i++)
	{
		if (this->contents[i] == MV_TILE_COIN)
			this->coins++;
	}

	fclose(lvlfile);

	return;

error:
	this->width  = 0;
	this->height = 0;
	this->player_x = 0;
	this->player_y = 0;

	free(this->contents);
	this->contents = NULL;

	if (lvlfile) fclose(lvlfile);

	return;
}

void Level :: save(char *filename)
{
	FILE *outfile = NULL;

	lvl_check(filename, "Null-pointer passed to Level::save for 'char *filename'.");

	outfile = fopen(filename, "w");
	lvl_check(filename, "Error writing file.");

	fputc(0xA6, outfile);
	fputc(0xC4, outfile);
	fputs("MVGAME", outfile);	// Does not include null-term
	fputc(0x01, outfile);
	fputc(0x00, outfile);
	fputc((uint8_t) this->width , outfile);
	fputc((uint8_t) this->height, outfile);
	fputc((uint8_t) this->player_x, outfile);
	fputc((uint8_t) this->player_y, outfile);
	fwrite(this->name, sizeof(char), LVL_NAMELEN, outfile);
	fputc(0x00, outfile);

	/* Write the level data */

	uint8_t byte;
	int index;

	while (index < this->width * this->height)
	{
		byte = (this->contents[index] << 4);

		if (index != (this->width * this->height) - 1)
		{
			byte |= (this->contents[index + 1]);
		}

		/* First tile */
		fputc(byte, outfile);
		index += 2;
	}

	this->checksum = calculate_checksum();
	fputc(this->checksum, outfile);

	fclose(outfile);
	return;

error:
	fclose(outfile);
}

uint8_t Level :: tile_at(int x, int y)
{
	if (x > this->width || y > this->height)
	{
		return 0x10;	// Impossible 4-bit value to signify an error
	}

	return this->contents[(y * this->height) + x];
}

uint8_t* Level :: ptr_tile_at(int x, int y)
{
	if (x > this->width || y > this->height)
	{
		return NULL;
	}

	return &(this->contents[(y * this->height) + x]);
}

uint8_t Level :: tile_by(int x, int y, enum Direction direction)
{
	switch (direction)
	{
		case NORTH:
			return this->contents[( (y - 1) * this->height) + x];
		break;
		case SOUTH:
			return this->contents[( (y + 1) * this->height) + x];
		break;
		case EAST:
			return this->contents[(y * this->height) + (x + 1)];
		break;
		case WEST:
			return this->contents[(y * this->height) + (x - 1)];
		break;
	}
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
				tb_change_cell(tl_x + x, tl_y + y, ' ', TB_DEFAULT, TB_BLACK);
				break;
			case MV_TILE_GROUND:
				tb_change_cell(tl_x + x, tl_y + y, ' ', TB_DEFAULT, TB_BLUE);
				break;
			case MV_TILE_WALL:
				tb_change_cell(tl_x + x, tl_y + y, '#', TB_WHITE, TB_BLUE);
				break;
			case MV_TILE_COIN:
				tb_change_cell(tl_x + x, tl_y + y, '+', TB_GREEN | TB_BOLD, TB_BLUE);
				break;
			case MV_TILE_EXIT:
				tb_change_cell(tl_x + x, tl_y + y, (this->exits_open ? 'O' : 'X'), (this->exits_open ? TB_GREEN : TB_RED) | TB_BOLD, TB_BLUE);
				break;

			default:
				tb_change_cell(tl_x + x, tl_y + y, '@', TB_WHITE | TB_BOLD, TB_RED);
				break;
			}

		}
	}

}

uint8_t Level :: calculate_checksum()
{
	uint32_t sum = 0;

	for (int i = 0; i < this->width * this->height; i++)
	{
		sum += this->contents[i];
	}

	return (uint8_t) sum % 256;
}

int Level :: check()
{
	return (this->checksum == this->calculate_checksum());
}
