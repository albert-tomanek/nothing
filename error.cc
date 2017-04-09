#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <termbox.h>

#include "error.hh"

#include "misc.hh"

#define ERRMSG_HDR   "ERROR"
#define ERRMSG_OKTXT "OK"

void mv_error(char *message, enum mv_ErrorFormat format)
{
	if (format == TERMBOX)
	{
		/* Draw the error box */
				
		tb_fill(tb_width() / 2 - strlen(message) / 2 - 2, tb_height() / 2 - 1, tb_width() / 2 + strlen(message) / 2 + 2, tb_height() / 2 + 3, TB_RED);
		
		for (int x = tb_width() / 2 - strlen(message) / 2 - 2; x <= tb_width() / 2 + strlen(message) / 2 + 2; x++)
		{
			tb_change_cell(x, tb_height() / 2 - 2, ' ', TB_DEFAULT | TB_UNDERLINE, TB_RED);
		}		
		
		/* Print the header and message */
		tb_print(tb_width() / 2 - strlen(ERRMSG_HDR) / 2, tb_height() / 2 - 2, ERRMSG_HDR, TB_WHITE | TB_BOLD | TB_UNDERLINE, TB_RED);

		tb_print(tb_width() / 2 - strlen(message) / 2, tb_height() / 2, message, TB_WHITE, TB_RED);
		
		/* Create the button */
		
		tb_fill(tb_width() / 2 - strlen(ERRMSG_OKTXT) / 2 - 2, tb_height() / 2 + 2, tb_width() / 2 + strlen(ERRMSG_OKTXT) / 2 + 2, tb_height() / 2 + 2, TB_CYAN);
		
		tb_print(tb_width() / 2 - strlen(ERRMSG_OKTXT) / 2, tb_height() / 2 + 2, ERRMSG_OKTXT, TB_WHITE , TB_CYAN);
		
		tb_present();
		
		/* Wait until they press enter */
		
		struct tb_event event;
		do
		{
			tb_poll_event(&event);
		}
		while (event.key != TB_KEY_ENTER);
		
		/* Highlight the button text to indicate that it has been pressed. */
		
		tb_print(tb_width() / 2 - strlen(ERRMSG_OKTXT) / 2, tb_height() / 2 + 2, ERRMSG_OKTXT, TB_WHITE | TB_BOLD, TB_CYAN);
		tb_present();
		
		tb_sleep(100);
		
		tb_shutdown();
	}
	else
	{
		fprintf(stderr, "Error: %s\n", message);
		getchar();
	}
}
