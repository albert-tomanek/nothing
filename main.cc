#include <termbox.h>

#include "error.hh"

int main()
{
	tb_init();
	mv_error("We are really sorry, but a fatal error occured.", TERMBOX);
	tb_shutdown();
		
	return 0;
}
