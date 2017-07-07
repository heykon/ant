#ifndef ANT_H
#define ANT_H

#include "something.h"


class ant: public Something
{
private:
	Something* target;

	bool moving;
	dir move_dir;
	int speed;
public:
	ant(Vec2d start_pos, int start_anim, SDL_Renderer* r, Interaction_system<Something*>* o, Something* t)
		: Something({30, 30}, start_pos, {50, 50}, start_anim, "ant.txt", r, o)
		, target(t)
		, speed(5)
		, moving(false)
		, move_dir(NONE)
	{set_prop(ANT);}

	void prepare_for_next_frame();
};

#endif