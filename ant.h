#ifndef ANT_H
#define ANT_H

#include "something.h"

class Ant : public Something
{
private:
	bool moving;
	dir move_dir;
	int speed;
	bool attacking;
	int atpr;
	int health;
public:
	Ant(Vec2d start_pos, int start_anim, SDL_Renderer* r, Interaction_system<Something*>* o)
			: Something({30, 30}, start_pos, {50, 50}, start_anim, "ant.txt", r, o)
			, speed(5)
			, moving(false)
			, move_dir(NONE)
		{set_prop(ANT);}
	void damaged();
};

#endif