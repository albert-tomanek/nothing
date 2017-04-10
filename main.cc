#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termbox.h>

#include "level.hh"
#include "player.hh"
#include "error.hh"
#include "misc.hh"
#include "tiles.h"

#define SHORT_STRLEN 16

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Syntax:\n\t%s <level.mv>\n", argv[0]);
	}

	tb_init();

	/* Load the level */
	Level *lvl = new Level(argv[1]);
	if ( lvl->error)   { mv_error(lvl->error, TERMBOX); free(lvl->error); tb_shutdown(); exit(1);}
	if (!lvl->check()) { mv_error("Checksum failed.", TERMBOX); tb_clear(); }

	/* Create the player */
	Player *player = new Player();
	player->x = lvl->player_x;
	player->y = lvl->player_y;

	/* Keypress event struct */
	struct tb_event *keypress = malloc(sizeof(struct tb_event));

	/* Other variables */
	char coords_str[SHORT_STRLEN];		// Just used to sprintf() the player's coordinates to.

	int run = 1;
	while (run)
	{
		/* Clear the screen before re-drawing */
		tb_clear();

		/* Draw the level */
		lvl->draw(tb_width() / 2 - player->x, tb_height() / 2 - player->y);

		/* Draw the character */
		player->draw_xy(tb_width() / 2, tb_height() / 2);

		/* Draw the info bar */
		tb_fill(0, tb_height() - 1, tb_width(), tb_height() - 1, TB_BLUE);
		tb_print(0, tb_height() - 1, "Press F2 to quit.", TB_WHITE, TB_BLUE);
		sprintf(coords_str, "(%d,%d)", player->x, player->y);
		tb_print(tb_width()-strlen(coords_str), tb_height()-1, coords_str, TB_WHITE, TB_BLUE);

		/* Update the screen */
		tb_present();

		/* Wait for the next keypress */

		tb_poll_event(keypress);	// waits until a key is pressed

		switch (keypress->key)
		{
		case TB_KEY_ARROW_UP:
			player->symbol = '^';

			if (0 < player->y	&&												// If we're not on the northenmost row already
				lvl->tile_by(player->x, player->y, NORTH) == MV_TILE_GROUND	||	// And the tile north of us is a ground or a coin tile
				lvl->tile_by(player->x, player->y, NORTH) == MV_TILE_COIN	)
			{
				player->move(NORTH);
			}
			break;

		case TB_KEY_ARROW_DOWN:
			player->symbol = 'v';

			if (player->y < lvl->height - 1		&								// If we're not on the southernmost row already
				lvl->tile_by(player->x, player->y, SOUTH) == MV_TILE_GROUND	||	// And the tile south of us is a ground or a coin tile
				lvl->tile_by(player->x, player->y, SOUTH) == MV_TILE_COIN	)
			{
				player->move(SOUTH);
			}
			break;

		case TB_KEY_ARROW_LEFT:
			player->symbol = '<';

			if (0 < player->x	&&
				lvl->tile_by(player->x, player->y, WEST) == MV_TILE_GROUND	||
				lvl->tile_by(player->x, player->y, WEST) == MV_TILE_COIN	)
			{
				player->move(WEST);
			}
			break;

		case TB_KEY_ARROW_RIGHT:
			player->symbol = '>';

			if (player->x < lvl->width - 1	&&
				lvl->tile_by(player->x, player->y, EAST) == MV_TILE_GROUND	||
				lvl->tile_by(player->x, player->y, EAST) == MV_TILE_COIN	)
			{
				player->move(EAST);
			}
			break;


		/* To quit */

		case TB_KEY_F2:
			run = 0;
			break;
		}
	}

	/* End things */
	delete lvl;

	tb_shutdown();

	return 0;
}
