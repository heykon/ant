#define _WIN32_WINNT 0x502

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <unistd.h>

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "../../anim.h"
#include "../../something.h"
#include "../../interaction.h"

#include "../utilities.h"



#define RESEND_INTERVAL 600


struct ant_info
{
	Something* s;
	dir move_dir;
	bool moving, attacking, new_dir, valid;
	int atpr;
};

enum request
{
	POS_INFO = 1, 
	ACTION, 
	QUIT, 
	JOIN, 
	ACTION_INFO,
	ACTION_UPDATE
};

int main(int argc, char* args[])
{
	printf("Type ip address\n");
	// initialize network

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		printf("fail\n");
		exit(1);
	}

	char ip[20];
	scanf("%s", ip);

	printf("%s\n", ip);

	int s;
	struct sockaddr_in host, local;

	host.sin_family = AF_INET;
	host.sin_port = htons(9001);
	host.sin_addr.s_addr = inet_addr(ip);

	local.sin_family = AF_INET;
	local.sin_port = htons(9000);
	local.sin_addr.s_addr = INADDR_ANY;

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	bind(s, (struct sockaddr*)&local, sizeof(local));

	printf("1\n");

	char b[B_SIZE], rb[B_SIZE];
	memset(b, 0, B_SIZE);
	memset(rb, 0, B_SIZE);

	// initialize graphics


	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* w = SDL_CreateWindow("haaaa!!!!!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
	IMG_Init(IMG_INIT_PNG);

	SDL_Surface* img = IMG_Load("geography.png");
	SDL_Texture* background = SDL_CreateTextureFromSurface(r, img);
	SDL_FreeSurface(img);

	Interaction_system<Something*> occ(50, "geography.bmp");

	Something tree1({25, 25}, {500, 370}, {100, 200}, 0, "tree.txt", r, &occ);

	printf("2\n");

	struct timeval t;
	t.tv_sec = 1;
	t.tv_usec = 0;

	fd_set f;

	FD_ZERO(&f);
	FD_SET(s, &f);


	b[0] = JOIN;

	while (rb[0] != JOIN)
	{
		do
		{
			sendto(s, b, 1, 0, (struct sockaddr*)&host, sizeof(host));
		} while ((select(s, &f, NULL, NULL, &t) == 0));
		
		recvfrom(s, rb, 12, 0, NULL, NULL);
	}

	printf("3\n");

	if (rb[1] == 0)
	{
		close(s);
		WSACleanup();

		SDL_DestroyRenderer(r);
		SDL_DestroyWindow(w);
		w = NULL;
		r = NULL;
		IMG_Quit();
		SDL_Quit();

		exit(1);
	}

	printf("4\n");

	int last_id = rb[2];
	int id = rb[1];

	b[0] = POS_INFO;

	while (rb[0] != POS_INFO)
	{
		do
		{
			sendto(s, b, 1, 0, (struct sockaddr*)&host, sizeof(host));
		} while ((select(s, &f, NULL, NULL, &t) == 0));

		recvfrom(s, rb, (last_id*2 + 2)*ONE_INT, 0, NULL, NULL);
	}
	printf("5\n");

	std::vector<struct ant_info> ants;
	ants.push_back({NULL, NONE, false, false, false, false, 0});

	for (int i = 1; i <= last_id; ++i)
	{
		int x = char_b_to_int(rb, i*2);
		int y = char_b_to_int(rb, i*2 + 1);

		struct ant_info new_ant = {
			NULL,
			NONE,
			false,
			false,
			false,
			false,
			0
		};

		if (x != -1)
		{
			new_ant.valid = true;
			new_ant.s = new Something({30, 30}, {x, y}, {50, 50}, 0, "ant.txt", r, &occ);

		}
		ants.push_back(new_ant);
	}

	b[0] = ACTION_INFO;

	printf("6\n");

	while (rb[0] != ACTION_INFO)
	{
		do
		{
			sendto(s, b, 1, 0, (struct sockaddr*)&host, sizeof(host));
		} while ((select(s, &f, NULL, NULL, &t) == 0));
		
		recvfrom(s, rb, last_id*2 + 2, 0, NULL, NULL);
	}

	for (int i = 1; i <= last_id; ++i)
	{
		if (ants[i].valid)
		{
			ants[i].move_dir = (dir)rb[i*2];
			ants[i].moving = (bool)rb[i*2 + 1];
		}
	}	
	printf("7\n");

	bool game_over = false;
	SDL_Event ia;
	int frame = 0;
	const Uint8 *state;

	int speed = 5;
	bool ttm = false;

	unsigned char count = 1, resend_count = 0, msg_id = 0;
	char prev_msg[B_SIZE];
	memset(prev_msg, 0, B_SIZE);

	u_long im = 1;
	ioctlsocket(s, FIONBIO, &im);

	printf("%d\n", id);

	printf("8\n");

	while (!game_over)
	{
		while ((recvfrom(s, rb, B_SIZE, 0, NULL, NULL)) != -1)
		{

			int leave_id = 0, new_id = 0, x = 0, y = 0, sender_id = 0;

			switch (rb[0])
			{
				case POS_INFO:
				for (int i = 1; i <= last_id; ++i)
				{
					int new_x = char_b_to_int(rb, i*2);
					int new_y = char_b_to_int(rb, i*2 + 1);

					if (new_x != -1)
					{
						if (!ants[i].valid)
						{
							ants[i].s = new Something({30, 30}, {new_x, new_y}, {50, 50}, 0, "ant.txt", r, &occ);
							ants[i].valid = true;
							ants[i].new_dir = false;
							ants[i].moving = false;
							ants[i].move_dir = UP;
							ants[i].attacking = false;
							ants[i].atpr = 0;

						}
						else
						{
							ants[i].s->set_pos(new_x, new_y);
						}
					} else if(ants[i].valid)
					{
						delete ants[i].s;

						ants[i].s = NULL;
						ants[i].valid = false;
						ants[i].move_dir = NONE;
						ants[i].moving = false;
						ants[i].new_dir = false;
						ants[i].attacking = false;
						ants[i].atpr = 0;
					}
				}
				break;

				case ACTION:

				if ((unsigned char)rb[1] == count)
				{
					//resend_count = 0;
					count++;
					if (rb[3] == 1)
						ttm = true;
					else if (rb[3] == 2)
						ttm = false;
					//printf("ttm set to %d\n", ttm);
				}

				break;

				case QUIT:
				leave_id = rb[1];

				delete ants[leave_id].s;
				if (leave_id == last_id)
				{
					ants.pop_back();
					int i = last_id;

					while (!ants[--i].valid)
					{
						delete ants[i].s;
						ants.pop_back();
					}
					last_id = i;
				} else
				{
					ants[leave_id].valid = false;
					ants[leave_id].s = NULL;
				}

				break;

				case JOIN:
				new_id = rb[1];

				x = char_b_to_int(rb, 1);
				y = char_b_to_int(rb, 2);

				struct ant_info new_ant;
				if (new_id > last_id)
				{
					for (int i = last_id + 1; i <= new_id; ++i)
					{

						struct ant_info new_ant = {
							NULL,
							NONE,
							false,
							false,
							false,
							false,
							0
						};

						ants.push_back(new_ant);
					}
					last_id = new_id;
				} else
				{
					if (ants[new_id].valid)
						delete ants[new_id].s;
				}

				ants[new_id].s = new Something({30, 30}, {x, y}, {50, 50}, 0, "ant.txt", r, &occ);
				ants[new_id].moving = false;
				ants[new_id].new_dir = false;
				ants[new_id].valid = true;
				ants[new_id].move_dir = UP;
				ants[new_id].attacking = false;
				ants[new_id].atpr = 0;

				break;

				case ACTION_INFO:
				break;

				case ACTION_UPDATE:
				sender_id = rb[1];

				if (rb[4] == 1)
				{
					if (!ants[sender_id].attacking)
					{
						ants[sender_id].new_dir = true;
						ants[sender_id].attacking = true;
						ants[sender_id].moving = false;
					}
				} else
				{
					if (ants[sender_id].move_dir != (dir)rb[2])
					{
						ants[sender_id].move_dir = (dir)rb[2];
						ants[sender_id].new_dir = true;					
					}

					ants[sender_id].moving = (bool)rb[3];
				}

			}
		}


		SDL_PollEvent(&ia);

		state = SDL_GetKeyboardState(NULL);

		if (ia.type == SDL_QUIT)
		{
			close(s);
			WSACleanup();

			SDL_DestroyTexture(background);
			SDL_DestroyRenderer(r);
			SDL_DestroyWindow(w);
			w = NULL;
			r = NULL;
			IMG_Quit();
			SDL_Quit();

			for (int i = 1; i <= last_id; ++i)
			{
				if (ants[i].valid)
				{
					delete ants[i].s;					

				}
				
			}

			return 0;
		}


		int l = 10;
		bool send = false;

		if (ttm)
		{
			switch (ants[id].move_dir)
			{
				case UP:
				if (!state[SDL_SCANCODE_UP])
				{
					send = true;

				}
				break;

				case DOWN:
				if (!state[SDL_SCANCODE_DOWN])
				{
					send = true;

				}
				break;

				case LEFT:
				if (!state[SDL_SCANCODE_LEFT])
				{
					send = true;

				}
				break;

				case RIGHT:					
				if (!state[SDL_SCANCODE_RIGHT])
				{
					send = true;

				}
			}
		}

		if (state[SDL_SCANCODE_UP])
		{
			b[3] = 1;

			if (ants[id].move_dir != UP)
			{
				send = true;
				b[2] = UP;
			} else if (!ttm)
			{
				send = true;
			}

		} else if (state[SDL_SCANCODE_DOWN])
		{
			b[3] = 1;

			if (ants[id].move_dir != DOWN)
			{
				send = true;
				b[2] = DOWN;
			} else if (!ttm)
			{
				send = true;
			}


		} else if (state[SDL_SCANCODE_LEFT])
		{
			b[3] = 1;

			if (ants[id].move_dir != LEFT)
			{
				send = true;
				b[2] = LEFT;
			} else if (!ttm)
			{
				send = true;
			}


		} else if (state[SDL_SCANCODE_RIGHT])
		{
			b[3] = 1;

			if (ants[id].move_dir != RIGHT)
			{
				send = true;
				b[2] = RIGHT;
			} else if (!ttm)
			{
				send = true;
			}

		} else
		{
			b[3] = 2;
		}
/*
		if (ia.type == SDL_KEYDOWN
			&& ia.key.keysym.sym == SDLK_SPACE
			&& !ants[id].attacking)
		{
			send = true;
			b[4] = 1;
		} else
		{
			b[4] = 0;
		}
*/
		if (send)
		{
			b[0] = ACTION;
			b[1] = count;

			if (!memcmp(&prev_msg[2], &b[2], 3))
			{
				if ((++resend_count % RESEND_INTERVAL) == 0)
				{
					b[B_SIZE - 1] = msg_id++;
					sendto(s, b, 5, 0, (sockaddr*)&host, sizeof(host));
					printf("resent 2, dir %d, mov %d\n", b[2], b[3]);
				} else
				{
					printf("wait, %d %d %d\n", b[2], b[3], b[4]);
					printf("prev, %d %d %d\n", prev_msg[2], prev_msg[3], prev_msg[4]);
				}
			} else
			{
				b[B_SIZE - 1] = msg_id++;
				sendto(s, b, 5, 0, (sockaddr*)&host, sizeof(host));
				printf("sent 2, dir %d, mov %d\n", b[2], b[3]);
				resend_count = 0;
			}
			memcpy(prev_msg, b, 5);
		} else
		{
			resend_count = 0;
		}

		SDL_RenderClear(r);

		if(!(++frame%FRAME_INTERVAL))
		{

			for (int i = 0; i <= last_id; ++i)
			{
				if (ants[i].valid)
				{
					if (ants[i].new_dir)
					{
						switch (ants[i].move_dir)
						{
							case UP:
							if (ants[i].attacking)
								ants[i].s->change_anim(4);

							if (ants[i].moving)
								ants[i].s->change_anim(0);
							break;

							case DOWN:
							if (ants[i].attacking)
								ants[i].s->change_anim(5);

							if (ants[i].moving)
								ants[i].s->change_anim(1);
							break;

							case LEFT:
							if (ants[i].attacking)
								ants[i].s->change_anim(6);

							if (ants[i].moving)
								ants[i].s->change_anim(2);
							break;

							case RIGHT:
							if (ants[i].attacking)
								ants[i].s->change_anim(7);

							if (ants[i].moving)
								ants[i].s->change_anim(3);
						}
						ants[i].new_dir = false;
					}

					if (ants[i].attacking)
					{
						if (ants[i].atpr < 4)
						{
							ants[i].atpr++;
							ants[i].s->update_anim();
						} else 
						{
							ants[i].atpr = 0;
							ants[i].attacking = false;
							switch (ants[i].move_dir)
							{
								case UP:
								ants[i].s->change_anim(0);
								break;

								case DOWN:
								ants[i].s->change_anim(1);
								break;

								case LEFT:
								ants[i].s->change_anim(2);
								break;

								case RIGHT:
								ants[i].s->change_anim(3);
							}
						}
					} else if (ants[i].moving)
					{
						ants[i].s->update_anim();
						ants[i].s->move(ants[i].move_dir, speed);				
					}
				}
			}


		}

		SDL_RenderCopy(r, background, NULL, NULL);

		for (int i = 1; i <= last_id; ++i)
		{
			if (ants[i].valid)
				ants[i].s->render_anim();
		}
		tree1.render_anim();

		SDL_RenderPresent(r);


	}

	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	w = NULL;
	r = NULL;
	IMG_Quit();
	SDL_Quit();


	for (int i = 1; i <= last_id; ++i)
	{
		if (ants[i].valid)
		{
			delete ants[i].s;					

		}
		
	}

	close(s);
	WSACleanup();

	return 0;
}