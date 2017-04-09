#ifndef __MVGAME_PLAYER_H__
#define __MVGAME_PLAYER_H__

#define PLAYER_UP		'^'
#define PLAYER_DOWN		'v'
#define PLAYER_LEFT		'<'
#define PLAYER_RIGHT	'>'

enum Direction {
	NORTH,
	SOUTH,
	EAST,
	WEST
};

class Player
{	
public:
	int x, y;		// x and y of the player IN THE LEVEL
	char symbol;	// The symbol that represents the player
	
	Player();
	
	void move(enum Direction direction);
	
	void draw();
	void draw_xy(int x, int y);	// Like draw() but with custon (x,y)
};

#endif
