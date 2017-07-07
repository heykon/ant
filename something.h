#ifndef SOMETHING_H
#define SOMETHING_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "anim.h"
#include "interaction.h"



typedef Animation* animptr;

enum dir {NONE, UP, LEFT, DOWN, RIGHT};

enum property {INANIMATE, ANT};


class Something
{
private:
	Animation** anims;
	int current_anim;
	int num_anim;

	Vec2d col_size = {30, 30};

	int pos_x;
	int pos_y;

	Interaction_system<Something*>* occ;
	int id;
	static int id_c;
	property prop = INANIMATE;

	SDL_Renderer* rend;
public:
	Something(Vec2d col_size, Vec2d start_pos, Vec2d anim_size, int start_anim, const char* spec_file, SDL_Renderer* r, Interaction_system<Something*>* o);
	~Something();

	property get_prop();
	void set_prop(property p);

	bool test_collide(dir d, int l);
	void move(dir d, int l);
	int get_pos(int a);
	void set_pos(int x, int y);

	void change_anim(int new_anim);
	void update_anim();
	void render_anim();


};

#endif