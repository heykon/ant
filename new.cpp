#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include "anim.h"
#include "something.h"
#include "ant.h"
#include "interaction.h"

const int HEIGHT = 600;
const int WIDTH = 800;

SDL_Renderer* r = NULL;
SDL_Window* w = NULL;


int main(int argc, char* args[])
{

	int num_ants;

	printf("How many ants? ");
	scanf("%d", &num_ants);
	
	while(num_ants > 23)
	{
		printf("Too many\n");
		scanf("%d", &num_ants);
	}



	SDL_Init(SDL_INIT_VIDEO);
	w = SDL_CreateWindow("haaaa!!!!!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
	IMG_Init(IMG_INIT_PNG);

	SDL_Surface* img = IMG_Load("geography.png");
	SDL_Texture* background = SDL_CreateTextureFromSurface(r, img);
	SDL_FreeSurface(img);

	Interaction_system<Something*> occ(50, "geography.bmp");//, WIDTH, HEIGHT);


	Something tree1({25, 25}, {500, 370}, {100, 200}, 0, "tree.txt", r, &occ);

	Something ant1({30, 30}, {150, 400}, {50, 50}, 0, "ant.txt", r, &occ);

	std::vector<ant*> ants;


	int spawn = 215;

	char b[sizeof(ant)];
	Something* prev = new ant({spawn - 55, 100}, 0, r, &occ, (Something*)b);
	ants.push_back((ant*)prev);

	for(int i = 0; (i < num_ants - 2) && (i < 10); ++i)
	{
		prev = new ant({spawn, 100}, 0, r, &occ, prev);
		ants.push_back((ant*)prev);
		spawn += 55;
	}
	for(int i = 0; i < num_ants - 12; ++i)
	{
		prev = new ant({spawn, 200}, 0, r, &occ, prev);
		ants.push_back((ant*)prev);
		spawn -= 55;
	}

	Something* f = new (b) ant({spawn, 200}, 0, r, &occ, prev);
	ants.push_back((ant*)f);


	bool game_over = false;
	SDL_Event ia;
	int frame = 0;
	const Uint8 *state;

	bool moving = false;
	dir move_dir = NONE;
	int speed = 5;

	bool attacking = false;
	int kill = 0;


	while(!game_over)
	{

		SDL_PollEvent(&ia);

		state = SDL_GetKeyboardState(NULL);

		if (ia.type == SDL_QUIT)
		{
			SDL_DestroyRenderer(r);
			SDL_DestroyWindow(w);
			w = NULL;
			r = NULL;
			IMG_Quit();
			SDL_Quit();
			return 0;
		}

		int l = 10;
		if (ia.type == SDL_KEYDOWN)//state[SDL_SCANCODE_SPACE])
		{
			if (ia.key.keysym.sym == SDLK_SPACE)
			{
				if (!attacking)
				{
					attacking = true;

					if (moving)
						moving = false;
					switch (move_dir)
					{
						case UP:
						ant1.change_anim(4);
						break;

						case DOWN:
						ant1.change_anim(5);
						break;

						case LEFT:
						ant1.change_anim(6);
						break;

						case RIGHT:
						ant1.change_anim(7);
						break;												
					}
				}

			} else if (ia.key.keysym.sym == SDLK_m)
			{
				for (int i = 0; i < (HEIGHT / l); ++i)
				{
					printf("\n");
					for (int j = 0; j < (WIDTH / l); ++j)
					{
						printf("%d", occ.get_id(j*l, i*l));
					}
				}
			}
		}
		//SDL_SetRenderDrawColor(r, 0x00, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(r);

		if(!(++frame%2))
		{

			if (attacking)
			{
				if (kill < 4)
				{
					kill++;
					ant1.update_anim();
				} else 
				{
					kill = 0;
					attacking = false;
					switch (move_dir)
					{
						case UP:
						ant1.change_anim(0);
						break;

						case DOWN:
						ant1.change_anim(1);
						break;

						case LEFT:
						ant1.change_anim(2);
						break;

						case RIGHT:
						ant1.change_anim(3);
					}
				}

			} else
			{
				if (state[SDL_SCANCODE_UP])
				{
					if (move_dir != UP)
					{
						ant1.change_anim(0);
						move_dir = UP;
					}
					if (!ant1.test_collide(UP, speed))
						moving = true;
					else
						moving = false;

				} else if (state[SDL_SCANCODE_DOWN])
				{
					if (move_dir != DOWN)
					{
						ant1.change_anim(1);
						move_dir = DOWN;
					}
					if (!ant1.test_collide(DOWN, speed))
						moving = true;
					else
						moving = false;

				} else if (state[SDL_SCANCODE_LEFT])
				{
					if (move_dir != LEFT)
					{
						ant1.change_anim(2);
						move_dir = LEFT;
					}
					if (!ant1.test_collide(LEFT, speed))
						moving = true;
					else
						moving = false;

				} else if (state[SDL_SCANCODE_RIGHT])
				{
					if (move_dir != RIGHT)
					{
						ant1.change_anim(3);
						move_dir = RIGHT;
					}
					if (!ant1.test_collide(RIGHT, speed))
						moving = true;
					else
						moving = false;
				} else
				{
					moving = false;
				}
			}


			if (kill == 3)
			{
				int temp_id;
				switch (move_dir)
				{
					case UP:
					printf("kill %d x %d y", ant1.get_pos(0) + 15, ant1.get_pos(1) - 15);
					temp_id = occ.get_id(ant1.get_pos(0) + 15, ant1.get_pos(1) - 25);
					break;

					case DOWN:
					printf("kill %d x %d y", ant1.get_pos(0) + 15, ant1.get_pos(1) + 45);
					temp_id = occ.get_id(ant1.get_pos(0) + 15, ant1.get_pos(1) + 45);
					break;

					case LEFT:
					printf("kill %d x %d y", ant1.get_pos(0) - 15, ant1.get_pos(1) + 15);
					temp_id = occ.get_id(ant1.get_pos(0) - 15, ant1.get_pos(1) + 15);
					break;

					case RIGHT:					
					printf("kill %d x %d y", ant1.get_pos(0) + 45, ant1.get_pos(1) + 15);
					temp_id = occ.get_id(ant1.get_pos(0) + 45, ant1.get_pos(1) + 15);
				}

				printf(" %d\n", temp_id);
				if (temp_id)
				{
					printf("die ");
					Something* dead = occ.access_obj_by_id(temp_id);
					if (dead == &ant1)
						printf("suicide");

					ants.erase(std::find(ants.begin(), ants.end(), dead));
					num_ants--;

					delete dead;
					printf("dead\n");
				}
			}



			if (moving)
			{
				ant1.update_anim();
				ant1.move(move_dir, speed);				
			}


			for(int i = 0; i < num_ants; ++i)
			{
				ants[i]->prepare_for_next_frame();
			}

		}


		SDL_RenderCopy(r, background, NULL, NULL);

		for(int i = 0; i < num_ants; ++i)
		{
			ants[i]->render_anim();
		}
		ant1.render_anim();

		tree1.render_anim();

		SDL_RenderPresent(r);

	}

	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	w = NULL;
	r = NULL;
	IMG_Quit();
	SDL_Quit();

	return 0;
}