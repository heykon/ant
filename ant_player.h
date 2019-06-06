#ifndef ANT_PLAYER_H
#define ANT_PLAYER_H

#include "ant.h"

class Ant_player : public Ant
{
private:
	bool ttm;
	bool new_dir;
	unsigned char count;
public:
	Ant_player(Vec2d start_pos, int start_anim, SDL_Renderer* r, Interaction_system<Something*>* o)
		: Ant(start_pos, start_anim, r, o)
			, ttm(false)
			, new_dir(false)
			, count(0)
		{}
	void damaged();
};

#endif