#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termbox.h>

#include "mvedit.hh"

#include "level.hh"
#include "player.hh"
#include "error.hh"
#include "misc.hh"
#include "tiles.hh"

#define SHORT_STRLEN 16

/* Window drawing */
void draw_hlpwin();

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
	
	if (argc == 2)
	{
		lvl->load(argv[1]);
		if ( lvl->error)   { mv_error(lvl->error, TERMBOX); free(lvl->error); tb_shutdown(); exit(1);}
		if (!lvl->check()) { mv_error("Checksum failed.", TERMBOX); tb_clear(); }
	}
	else
	{
		lvl->width  = atoi(argv[1]);
		lvl->height = atoi(argv[2]);
		
		lvl->contents = (uint8_t *) calloc(lvl->width * lvl->height, sizeof(uint8_t));
	}

	/* Create a struct to store the cursor position in */
	Cursor *cursor = new Cursor();

	/* Keypress event struct */
	struct tb_event *keypress = (struct tb_event *) malloc(sizeof(struct tb_event));
	
	/* Remember which windows are shown */
	bool show_hlpwin = false;
	
	/* Other variables */
	char coords_str[SHORT_STRLEN];		// Just used to sprintf() the player's coordinates to.
	struct tb_cell cursor_cell;

	int run = 1;
	while (run)
	{
		/* Clear the screen before re-drawing */
		tb_clear();

		/* Draw the level */
		tb_draw_box_wh(tb_width() / 2 - cursor->x - 1, tb_height() / 2 - cursor->y - 1, lvl->width + 1, lvl->height + 1, TB_WHITE, TB_DEFAULT);
		lvl->draw(tb_width() / 2 - cursor->x, tb_height() / 2 - cursor->y);

		/* Draw the cursor */
		mv_tile_create(lvl->tile_at(cursor->x, cursor->y), &cursor_cell, lvl->exits_open);
		cursor_cell.bg = TB_MAGENTA;
		tb_put_cell(tb_width() / 2, tb_height() / 2, &cursor_cell);

		sprintf(coords_str, "(%d,%d)", cursor->x, cursor->y);									// Coordinates
		tb_print(tb_width() - strlen(coords_str), tb_height() - 1, coords_str, TB_WHITE, TB_DEFAULT);
		
		/* Draw windows */
		if (show_hlpwin) draw_hlpwin();
		
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
		}
	}

	/* End things */
	tb_shutdown();

	free(keypress);

	delete lvl;
	delete cursor;

	return 0;
}

void draw_hlpwin()
{
	/* Draw the help window */
	
	int win_width  = 20;
	int win_height =  4;
	
	tb_fill(tb_width() - win_width, 0, tb_width(), win_height, TB_BLUE);
	tb_print(tb_width() - (win_width - 2), 1, "F2 = Quit        ", TB_WHITE, TB_BLUE);
	tb_print(tb_width() - (win_width - 2), 2, "H  = Help window ", TB_WHITE, TB_BLUE);
	tb_print(tb_width() - (win_width - 2), 3, "T  = Tile table  ", TB_WHITE, TB_BLUE);
}

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
