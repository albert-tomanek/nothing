/* Sorry, I *may* have done a couple of messy/unsafe	*
 * here, because I'm a human and I get tired.			*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termbox.h>

#include "mvedit.hh"

#include "bbar.hh"
#include "level.hh"
#include "player.hh"
#include "error.hh"
#include "misc.hh"
#include "tiles.hh"

#define SHORT_STRLEN 16

/* Window drawing */
void draw_hlpwin();
void draw_tilewin();

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Syntax:\n");
		printf("\t%s <file>\n", argv[0]);
		printf("\t%s <width> <height>\n", argv[0]);

		exit(0);
	}

	tb_init();

	/* Create a new level */
	Level *lvl = new Level();

	char *filename = NULL;		// Where we save the name of the file

	if (argc == 2)
	{
		//filename = strdup(argv[1]);
		lvl->load(argv[1]);
		if ( lvl->error)   { mv_error(lvl->error, TERMBOX); free(lvl->error); tb_shutdown(); exit(1);}
		if (!lvl->check()) { mv_error("Checksum failed.", TERMBOX); tb_clear(); }
	}
	else
	{
		lvl->width  = atoi(argv[1]);
		lvl->height = atoi(argv[2]);

		if (lvl->width > 255 || lvl->height > 255)
		{
			/* A level size larger than 255 would not 	*
			 * fit into a 8-bit integer used in the 	*
			 * level file format.						*/

			mv_error("Level width and height must be below 255.", TERMBOX);
			tb_shutdown();
			delete lvl;
			exit(0);
		}

		lvl->contents = (uint8_t *) malloc(lvl->width * lvl->height * sizeof(uint8_t));
		memset(lvl->contents, MV_TILE_GROUND, lvl->width * lvl->height);
	}

	/* GUI Elements */
	Cursor *cursor = new Cursor();	// Create a struct to store the cursor position in
	BottomBar *bbar = new BottomBar();
	bbar->bg = TB_DEFAULT;
	bbar->display("Press the H key for help.");

	/* Keypress event struct */
	struct tb_event *keypress = (struct tb_event *) malloc(sizeof(struct tb_event));

	/* Remember which windows are shown */
	bool show_hlpwin  = false;
	bool show_tilewin = false;

	/* Other variables */
	char coords_str[SHORT_STRLEN];		// Just used to sprintf() the player's coordinates to.
	struct tb_cell cursor_cell;
	uint8_t *tile;		// The tile that the cursor is currently above

	int run = 1;
	while (run)
	{
		/* Clear the screen before re-drawing */
		tb_clear();

		/* Draw the level */
		tb_draw_box_wh(tb_width() / 2 - cursor->x - 1, tb_height() / 2 - cursor->y - 1, lvl->width + 1, lvl->height + 1, TB_WHITE, TB_DEFAULT);
		lvl->draw(tb_width() / 2 - cursor->x, tb_height() / 2 - cursor->y);

		tb_change_cell((tb_width() / 2 - cursor->x) + lvl->player_x, (tb_height() / 2 - cursor->y) + lvl->player_y, '>', TB_GREEN, TB_BLUE);	// AND the player's start position in the level

		/* Draw the level name */
		if (lvl->name)
			tb_print(0, 0, lvl->name, TB_WHITE, TB_DEFAULT);

		/* Draw the cursor */
		mv_tile_create(lvl->tile_at(cursor->x, cursor->y), &cursor_cell, lvl->exits_open);
		cursor_cell.bg = TB_MAGENTA;
		tb_put_cell(tb_width() / 2, tb_height() / 2, &cursor_cell);

		tile = lvl->ptr_tile_at(cursor->x, cursor->y);

		/* Draw the bottom bar */
		bbar->draw();

		sprintf(coords_str, "(%d,%d)", cursor->x, cursor->y);									// Coordinates
		tb_print(tb_width() - strlen(coords_str), tb_height() - 1, coords_str, TB_WHITE, TB_DEFAULT);

		/* Draw windows */
		if (show_hlpwin)  draw_hlpwin();
		if (show_tilewin) draw_tilewin();

		/* Update the screen */
		tb_present();

		/* Wait for the next keypress */

		tb_poll_event(keypress);	// waits until a key is pressed

		switch (keypress->key)
		{
		case TB_KEY_ARROW_UP:
			if (0 < cursor->y)
				cursor->move(NORTH);
			break;

		case TB_KEY_ARROW_DOWN:
			if (cursor->y < lvl->height - 1)
				cursor->move(SOUTH);
			break;

		case TB_KEY_ARROW_LEFT:
			if (0 < cursor->x)
				cursor->move(WEST);
			break;

		case TB_KEY_ARROW_RIGHT:
			if (cursor->x < lvl->width - 1)
				cursor->move(EAST);
			break;


		/* To quit */

		case TB_KEY_F2:
			run = 0;
			break;
		}

		switch (keypress->ch)
		{
		case 'h':
			show_hlpwin = !show_hlpwin;
			break;
		case 't':
			show_tilewin = !show_tilewin;
			break;
		case 'n':
			bbar->query("Level name: ", lvl->name, LVL_NAMELEN);
			break;
		case 'p':
		{
			char intbuf[STRLEN];
			int x, y;

			memset(intbuf, '\0', STRLEN);
			bbar->query("Player start X: ", intbuf, STRLEN);
			x = atoi(intbuf);

			memset(intbuf, '\0', STRLEN);
			bbar->query("Player start Y: ", intbuf, STRLEN);
			y = atoi(intbuf);

			if (x > lvl->width || y > lvl->height  ||
				x < 0 || y < 0)
			{
				bbar->display("Coordinates out of level area!");
			}
			else if (lvl->tile_at(x, y) == MV_TILE_EMPTY)
			{
				bbar->display("Can't put player on an empty tile!");
			}
			else
			{
				lvl->player_x = x;
				lvl->player_y = y;
			}

			break;
		}
		case 's':
		{
			if (! filename) filename = (char*) calloc(STRLEN, sizeof(char));

			bbar->query("Save as: ", filename, STRLEN);

			lvl->save(filename);
			break;
		}

		/* Commands that change the current tile */
		case '0':
			(*tile) = MV_TILE_EMPTY;
			break;
		case '1':
			(*tile) = MV_TILE_WALL;
			break;
		case '2':
			(*tile) = MV_TILE_GROUND;
			break;
		case '3':
			(*tile) = MV_TILE_COIN;
			break;
		case '4':
			(*tile) = MV_TILE_EXIT;
			break;
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			//(*tile) = ;	// These tiles are not used yet
			break;
		}
	}

	/* End things */
	tb_shutdown();

	free(keypress);
	free(filename);

	delete lvl;
	delete cursor;

	return 0;
}

void draw_hlpwin()
{
	/* Draw the help window */

	int win_width  = 20;
	int win_height =  7;

	tb_fill(tb_width() - win_width, 1, tb_width(), 1 + win_height, TB_BLUE);
	tb_print(tb_width() - (win_width - 2), 2, "F2 = Quit        ", TB_WHITE, TB_BLUE);
	tb_print(tb_width() - (win_width - 2), 3, "H  = Help window ", TB_WHITE, TB_BLUE);
	tb_print(tb_width() - (win_width - 2), 4, "T  = Tile table  ", TB_WHITE, TB_BLUE);
	tb_print(tb_width() - (win_width - 2), 5, "N  = Level name  ", TB_WHITE, TB_BLUE);
	tb_print(tb_width() - (win_width - 2), 6, "P  = Player start", TB_WHITE, TB_BLUE);
	tb_print(tb_width() - (win_width - 2), 7, "S  = Save level  ", TB_WHITE, TB_BLUE);
}

void draw_tilewin()
{
	/* Draw the help window */

	int win_width  = 14;
	int win_height =  6;

	tb_fill(0, 1, win_width + 1, win_height + 1, TB_BLUE);
	tb_print(1, 2, "0 = EMPTY  ", TB_WHITE, TB_BLUE);
	tb_print(1, 3, "1 = WALL   ", TB_WHITE, TB_BLUE);
	tb_print(1, 4, "2 = GROUND ", TB_WHITE, TB_BLUE);
	tb_print(1, 5, "3 = COIN   ", TB_WHITE, TB_BLUE);
	tb_print(1, 6, "4 = EXIT   ", TB_WHITE, TB_BLUE);
}

/* Cursor */

Cursor :: Cursor()
{
	this->x = 0;
	this->y = 0;
}

void Cursor :: move(enum Direction direction)
{
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

/* Bottom bar */
