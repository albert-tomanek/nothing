#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <termbox.h>

#include "bbar.hh"

#include "misc.hh"

BottomBar :: BottomBar()
{
	this->message = NULL;
}
BottomBar :: ~BottomBar()
{
	if (this->message)
	{
		free(this->message);
	}
}

void BottomBar :: draw()
{
	tb_fill(0, tb_height() - 1, tb_width() - 1, tb_height() - 1, this->bg);

	if (this->message) tb_print(0, tb_height() - 1, this->message, TB_WHITE, this->bg);
}

void BottomBar :: display(char *message)
{
	if (this->message) free(this->message);

	this->message = strdup(message);
}

void BottomBar :: query(char *message, char *buffer, size_t buffer_size)
{
	/* Keypress event struct */
	struct tb_event *keypress = (struct tb_event *) malloc(sizeof(struct tb_event));

	do
	{
		/* Draw the text */
		tb_fill(0, tb_height() - 1, strlen(message) + buffer_size, tb_height() - 1, this->bg);
		tb_print(0, tb_height() - 1, message, TB_WHITE, this->bg);
		tb_print(strlen(message), tb_height() - 1, buffer, TB_WHITE, this->bg);
		tb_change_cell(strlen(message) + strlen(buffer), tb_height() - 1, '_', TB_WHITE | TB_BOLD, this->bg);

		tb_present();

		tb_poll_event(keypress);	// waits until a key is pressed

		/* React to keypress */
		if (keypress->ch)
		{
			if (strlen(buffer) < buffer_size)
			{
				buffer[strlen(buffer)] = keypress->ch;
			}
		}
		else if (keypress->key)
		{
			switch (keypress->key)
			{
				case (TB_KEY_BACKSPACE):
				case (TB_KEY_BACKSPACE2):
					buffer[strlen(buffer)-1] = '\0';	// Make the string one char shorter.
				break;

				case (TB_KEY_SPACE):
					buffer[strlen(buffer)] = ' ';
				break;
				case (TB_KEY_ENTER):
				{
					free(keypress);
					return;
				}
			}

		}
	}
	while (keypress->key != TB_KEY_ENTER);
}
