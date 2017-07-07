#include "something.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>


int Something::id_c = 1;


Something::Something(Vec2d col_size, Vec2d start_pos, Vec2d anim_size, int start_anim, const char* spec_file, SDL_Renderer* r, Interaction_system<Something*>* o)
	: occ(o)
	, rend(r)
	, pos_x(start_pos.x)
	, pos_y(start_pos.y)
	, col_size(col_size)
	, current_anim(start_anim)
	, id(id_c++)
{

	std::ifstream spec(spec_file);


	for (int i = 0; i < col_size.x; ++i)
	{
		for (int j = 0; j < col_size.y; ++j)
			occ->modify(pos_x + i, pos_y + j, id);
	}

	occ->insert_obj(id, this);

	std::string str;
	std::string temps;
	int tempi;
	std::vector<std::string> sv;
	std::vector<int> iv;

	int count = 0;
	while (getline(spec, str))
    {
    	std::stringstream ss(str);
    	ss >> temps >> tempi;
    	sv.push_back(temps);
    	iv.push_back(tempi);
    	++count;
    }
    num_anim = count;
    anims = new animptr[count];
    spec.close();

    rend = r;

	for (int i = 0; i < count; ++i)
		anims[i] = new Animation(anim_size, iv[i], sv[i], rend);
}

Something::~Something()
{
	for (int i = 0; i < num_anim; ++i)
	{
		delete anims[i];
	}
	delete [] anims;
	anims = NULL;

	for (int i = 0; i < col_size.x; ++i)
	{
		for (int j = 0; j < col_size.y; ++j)
			occ->modify(pos_x + i, pos_y + j, 0);
	}
}


void Something::move(dir d, int l)
{
	int start_x = pos_x;
	int start_y = pos_y;
	switch(d)
	{
		case UP:
		pos_y -= l;
		for (int i = 0; i < col_size.x; ++i)
		{
			for (int j = 1; j <= l; ++j)
			{
				occ->modify(start_x + i, start_y - j, id);
				occ->modify(start_x + i, start_y + col_size.y - j, 0);
			}
		}
		break;

		case LEFT:
		pos_x -= l;
		for (int i = 0; i < col_size.y; ++i)
		{
			for (int j = 1; j <= l; ++j)
			{
				occ->modify(start_x - j, start_y + i, id);
				occ->modify(start_x + col_size.x - j, start_y + i, 0);
			}
		}
		break;

		case DOWN:
		pos_y += l;
		for (int i = 0; i < col_size.x; ++i)
		{
			for (int j = 0; j < l; ++j)
			{
				occ->modify(start_x + i, start_y + col_size.y + j, id);
				occ->modify(start_x + i, start_y + j, 0);
			}
		}
		break;

		case RIGHT:
		pos_x += l;
		for (int i = 0; i < col_size.y; ++i)
		{
			for (int j = 0; j < l; ++j)
			{
				occ->modify(start_x + col_size.x + j, start_y + i, id);
				occ->modify(start_x + j, start_y + i, 0);
			}
		}
		break;
	}
}


void Something::set_pos(int x, int y)
{
	pos_y = y;
	pos_x = x;
}

void Something::change_anim(int new_anim)
{
	anims[current_anim]->reset();
	current_anim = new_anim;
}

void Something::update_anim()
{
	anims[current_anim]->update();
}

void Something::render_anim()
{
	Vec2d si = anims[current_anim]->get_size();
	anims[current_anim]->render_frame(rend, pos_y - (si.y - col_size.y)/2, pos_x - (si.x - col_size.x)/2);
}

int Something::get_pos(int a)
{
	return a ? pos_y : pos_x;
}

property Something::get_prop()
{
	return prop;
}
void Something::set_prop(property p)
{
	prop = p;
}


bool Something::test_collide(dir d, int l)
{
	int start_x = pos_x;
	int start_y = pos_y;

	switch(d)
	{
		case UP:
		if ((start_y - l) < 0)
			return true;
		for (int i = 0; i < col_size.x; ++i)
		{
			for (int j = 1; j <= l; ++j)
			{
				if (occ->get_id(start_x + i, start_y - j))
					return true;
			}
		}
		break;

		case DOWN:
		if ((start_y + col_size.y + l) >= occ->get_height())
			return true;
		for (int i = 0; i < col_size.x; ++i)
		{
			for (int j = 0; j < l; ++j)
			{
				if (occ->get_id(start_x + i, start_y + col_size.y + j))
					return true;
			}
		}	
		break;

		case LEFT:
		if ((start_x - l) < 0)
			return true;
		for (int i = 0; i < col_size.y; ++i)
		{
			for (int j = 1; j <= l; ++j)
			{
				if (occ->get_id(start_x - j, start_y + i))
					return true;
			}
		}		
		break;

		case RIGHT:
		if ((start_x + col_size.x + l) >= occ->get_width())
			return true;
		for (int i = 0; i < col_size.y; ++i)
		{
			for (int j = 0; j < l; ++j)
			{
				if (occ->get_id(start_x + col_size.x + j, start_y + i))
					return true;
			}
		}
		break;

	}
	return false;
	
}