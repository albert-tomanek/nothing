#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <termbox.h>

#include <unistd.h>

#include "misc.hh"
#include "chars.h"

/* An assortment of other functions,
 * which are used to draw stuff;
 * copy-pasted from libtermex...
 */

uint32_t *to_utf32(char *string) 				// Supponwdly works for ASCII only...
{
	int maxLen = strlen(string);
	uint32_t *out = (uint32_t*) malloc(sizeof(uint32_t) * maxLen);

	for (int i = 0; i < maxLen; i++)
	{
		if (string[i] == '\0')
		{
			out[i] = 0;
			break;
		}
		else
		{
			out[i] = (uint32_t) string[i];
		}
	}

	return out;
}

void tb_print(int x, int y, char *string, uint16_t fg, uint16_t bg)
{
	int string_len = strlen(string);

	/* Convert the string to unicode */

	uint32_t *string32 = to_utf32(string);	// Rememer to free!!

	/* Now print it out */

	for (int i = 0; i < string_len; i++)
	{
		tb_change_cell(x+i, y, string32[i], fg, bg);
	}

	free(string32);
}

void tb_draw_box(int tl_x, int tl_y, int br_x, int br_y, uint16_t fg, uint16_t bg)
{
	/* MAY NOT WORK							*
	 * This function has not been checked	*
	 * since major changes were made to		*
	 * tb_draw_box_wh().					*/

	int width  = br_x - tl_x;
	int height = br_y - tl_y;

	tb_change_cell(tl_x, tl_y, TB_BOX_se, fg, bg);	 			 // Top Left
	tb_change_cell(br_x, tl_y, TB_BOX_sw, fg, bg);	 			 // Top Right
	tb_change_cell(br_x, br_y, TB_BOX_nw, fg, bg);	 			 // Bottom Right
	tb_change_cell(tl_x, br_y, TB_BOX_ne, fg, bg);	 			 // Bottom Left

	for (int i = 0; i < (width - 1); i++)			 // Horizontals
	{
		tb_change_cell(1 + tl_x + i, tl_y, TB_BOX_ew, fg, bg);	 // N
		tb_change_cell(1 + tl_x + i, br_y, TB_BOX_ew, fg, bg);	 // S
	}

	for (int i = 0; i < (height - 1); i++)			 // Verticals
	{
		tb_change_cell(tl_x, 1 + tl_y + i, TB_BOX_ns, fg, bg);	 // E
		tb_change_cell(br_x, 1 + tl_y + i, TB_BOX_ns, fg, bg);	 // W
	}

	/* Also, we should fill the box with the given colour */

	tb_fill(tl_x, tl_y, br_x, br_y, bg);
}

void tb_draw_box_wh(int tl_x, int tl_y, int width, int height, uint16_t fg, uint16_t bg)
{
	int br_x = tl_x + width;
	int br_y = tl_y + height;

	/* Fill the area */
	tb_fill(tl_x, tl_y, br_x, br_y, bg);

	tb_change_cell(tl_x, tl_y, TB_BOX_se, fg, bg);	 			 // Top Left
	tb_change_cell(br_x, tl_y, TB_BOX_sw, fg, bg);	 			 // Top Right
	tb_change_cell(br_x, br_y, TB_BOX_nw, fg, bg);	 			 // Bottom Right
	tb_change_cell(tl_x, br_y, TB_BOX_ne, fg, bg);	 			 // Bottom Left

	for (int x = tl_x+1; x <= br_x-1; x++)			 // Horizontals
	{
		tb_change_cell(x, tl_y, TB_BOX_ew, fg, bg);	 // N
		tb_change_cell(x, br_y, TB_BOX_ew, fg, bg);	 // S
	}

	for (int y = tl_y+1; y <= br_y-1; y++)			 // Verticals
	{
		tb_change_cell(tl_x, y, TB_BOX_ns, fg, bg);	 // E
		tb_change_cell(br_x, y, TB_BOX_ns, fg, bg);	 // W
	}
}

void tb_fill(int tl_x, int tl_y, int br_x, int br_y, uint16_t bg)
{
	for (int x = tl_x; x <= br_x; x++)
	{
		for (int y = tl_y; y <= br_y; y++)
		{
			tb_change_cell(x, y, ' ', TB_DEFAULT, bg);
		}
	}
}

void tb_sleep(int millisecs)
{
	/* For portability */
	usleep(millisecs * 1000);
}

/* Text functions */

char *text_shorten(char *in, int len)
{
	char *out = strdup(in);

	out[len]    = '\0';
	out[len-1]  = '.';
	out[len-2]  = '.';
	out[len-3]  = '.';

	return out;
}

char *text_fill(char *in, int len)
{
	int in_len = strlen(in);

	char *out = calloc(len+1, sizeof(char));    //len+1 for the null-term at the end.

	if(in_len > len) 	// The text is _larger_ than tne area to be filled.
	{
		out = text_shorten(in, len);
	}
	else		// The text is _smaller_ than the area to be flled.
	{
		strcpy(out, in);

		for (int i = 0; i < (len - in_len); i++)
		{
			out[in_len + i] = ' ';
		}

	}

	return out;
}

