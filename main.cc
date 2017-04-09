#include <stdlib.h>
#include <termbox.h>

#include "level.hh"
#include "error.hh"

int main()
{
	//tb_init();
	
	Level *lvl = new Level("levels/lvl1.mv");
	if (lvl->error) { mv_error(lvl->error, STDIO); free(lvl->error); }
	
	printf("width: %d\theight: %d\nname: \"%s\"\nstart x: %d\tstart y: %d\n", lvl->width, lvl->height, lvl->name, lvl->player_x, lvl->player_y);
	
	//tb_shutdown();

	delete lvl;
	
	return 0;
}
