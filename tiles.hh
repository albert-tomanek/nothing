#ifndef __MVGAME_TILES_H__
#define __MVGAME_TILES_H__

/* Define tiles */

#define MV_TILE_EMPTY  0x0
#define MV_TILE_WALL   0x1
#define MV_TILE_GROUND 0x2
#define MV_TILE_COIN   0x3
#define MV_TILE_EXIT   0x4

/* Create a tile */
void mv_tile_create(uint8_t type, struct tb_cell *cell, bool exit_open);

#endif
