#include <stdlib.h>
#include <termbox.h>

#include "level.hh"
#include "error.hh"
#include "misc.hh"

int main()
{
	tb_init();
	
	Level *lvl = new Level("levels/lvl1.mv");
	if ( lvl->error)   { mv_error(lvl->error, TERMBOX); free(lvl->error); exit(1);}
	if (!lvl->check()) { mv_error("Checksum failed.", TERMBOX); tb_clear();}
	
	lvl->draw(1, 1);
	
	tb_present();
	tb_sleep(5000);
	tb_shutdown();

	delete lvl;
	
	return 0;
}
