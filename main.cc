#include <stdlib.h>
#include <termbox.h>

#include "level.hh"
#include "error.hh"

int main()
{
	//tb_init();
	
	Level *lvl = new Level("levels/lvl1.mv");
	if (lvl->error) { mv_error(lvl->error, STDIO); free(lvl->error); }
	
	for (int i = 0; i < lvl->width * lvl->height; i++)
	{
		printf("%02X ", lvl->contents[i]);
	}
	printf("\nChecksum %s.\n", lvl->check() ? "passed" : "failed");
	//tb_shutdown();

	delete lvl;
	
	return 0;
}
