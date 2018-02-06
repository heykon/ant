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
#include <deque>

#include "../../anim.h"
#include "../../something.h"
#include "../../interaction.h"

#include "../utilities.h"



struct player_info
{
	Something* s;
	dir move_dir;
	bool moving, ttm, new_dir;
	int id;
	unsigned char count;
};



int main(int argc, char* args[])
{

	// initialize network

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		printf("fail\n");
		exit(1);
	}

	int s;
	struct sockaddr_in client, local;

	client.sin_family = AF_INET;
	client.sin_port = htons(9000);

	local.sin_family = AF_INET;
	local.sin_port = htons(9001);
	local.sin_addr.s_addr = INADDR_ANY;

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	bind(s, (struct sockaddr*)&local, sizeof(local));


	u_long im = 1;
	ioctlsocket(s, FIONBIO, &im);

	char b[B_SIZE];
	memset(b, 0, B_SIZE);

	struct sockaddr_in src_addr;

	src_addr.sin_port = htons(9000);
	src_addr.sin_family = AF_INET;
	socklen_t len = sizeof(struct sockaddr);


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

	Something ant1({30, 30}, {300, 300}, {50, 50}, 0, "ant.txt", r, &occ);
	Something tree1({25, 25}, {500, 370}, {100, 200}, 0, "tree.txt", r, &occ);

	bool game_over = false;
	SDL_Event ia;
	int frame = 0;
	const Uint8 *state;

	bool moving = false;
	dir move_dir = UP;
	int speed = 5;

	std::map<u_long,struct player_info> players;
	int last_id = 1;
	std::deque<int> ids_not_in_use;



	while (!game_over)
	{

		while ((recvfrom(s, b, 4, 0, (struct sockaddr*) &src_addr, &len)) != -1)
		{
			auto sender_it = players.find(src_addr.sin_addr.s_addr);

			switch (b[0])
			{
				case 1:
				for (auto it = ids_not_in_use.begin(); it != ids_not_in_use.end(); ++it)
					int_to_char_b(b, *it*2, -1);

				for (auto it = players.begin(); it != players.end(); ++it)
				{
					int x = it->second.s->get_pos(0);
					int y = it->second.s->get_pos(1);

					int_to_char_b(b, it->second.id*2, x);
					int_to_char_b(b, it->second.id*2 + 1, y);

				}

				int_to_char_b(b, 2, ant1.get_pos(0));
				int_to_char_b(b, 3, ant1.get_pos(1));

				sendto(s, b, (last_id*2 + 2)*ONE_INT, 0, (struct sockaddr*)&src_addr, sizeof(src_addr));

				break;

				case 2:
				printf("server; recv 2 from %d, count %d, local count%d\n", sender_it->second.id, (unsigned char)b[1], sender_it->second.count);
				if ((unsigned char)b[1] == ++sender_it->second.count)
				{
					if (b[3] == 1)
					{
						sender_it->second.new_dir = sender_it->second.move_dir != (dir)b[2];
						sender_it->second.move_dir = (dir)b[2];
						sender_it->second.ttm = true;
					
					} else if (b[3] == 2)
					{
						sender_it->second.ttm = false;
					}
					printf("ttm set to %d\n", sender_it->second.ttm);					
					sendto(s, b, 4, 0, (struct sockaddr*)&src_addr, sizeof(src_addr));
					
				} else if ((unsigned char)b[1] == sender_it->second.count)
				{
					sendto(s, b, 4, 0, (struct sockaddr*)&src_addr, sizeof(src_addr));
				}

				break;

				case 3:
				if (sender_it == players.end())
				{
					sendto(s, b, 2, 0, (struct sockaddr*)&src_addr, sizeof(src_addr));
				} else
				{

					b[1] = sender_it->second.id;

					for (auto it = players.begin(); it != players.end(); ++it)
					{
						client.sin_addr.s_addr = it->first;
						sendto(s, b, 2, 0, (struct sockaddr*)&client, sizeof(client));
					}

					delete sender_it->second.s;
					if (sender_it->second.id == last_id)
						last_id--;
					else
						ids_not_in_use.push_back(sender_it->second.id);
					players.erase(sender_it);
				}
				break;

				case 4:
				if (sender_it == players.end())
				{
					struct player_info new_player;

					int x = 400;
					int y = 300;


					if (ids_not_in_use.empty())
					{
						new_player.id = ++last_id;
					} else
					{
						new_player.id = ids_not_in_use.front();
						ids_not_in_use.pop_front();
					}

					new_player.s = new Something({30, 30}, {x, y}, {50, 50}, 0, "ant.txt", r, &occ);
					new_player.moving = false;
					new_player.ttm = false;
					new_player.new_dir = false;
					new_player.move_dir = UP;
					new_player.count = 0;

					players[src_addr.sin_addr.s_addr] = new_player;


					b[1] = new_player.id;
					b[2] = last_id;

					int_to_char_b(b, 1, x);
					int_to_char_b(b, 2, y);

					for (auto it = players.begin(); it != players.end(); ++it)
					{
						client.sin_addr.s_addr = it->first;
						sendto(s, b, 12, 0, (struct sockaddr*)&client, sizeof(client));
					}
				} else
				{
					b[1] = sender_it->second.id;
					b[2] = last_id;

					int_to_char_b(b, 1, sender_it->second.s->get_pos(0));
					int_to_char_b(b, 2, sender_it->second.s->get_pos(1));

					client.sin_addr.s_addr = sender_it->first;
					sendto(s, b, 12, 0, (struct sockaddr*)&src_addr, sizeof(src_addr));
				}
				break;

				case 5:
				for (auto it = players.begin(); it != players.end(); ++it)
				{

					b[it->second.id*2] = it->second.move_dir;
					b[it->second.id*2 + 1] = it->second.moving;
				}

				b[2] = move_dir;
				b[3] = moving;

				sendto(s, b, last_id*2 + 2, 0, (struct sockaddr*)&src_addr, sizeof(src_addr));

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

			for (auto it = players.begin(); it != players.end(); ++it)
			{
				delete it->second.s;
			}
			return 0;
		}


		SDL_RenderClear(r);

		if (!(frame%600))
		{
			for (int i = 1; i <= last_id; ++i)
				int_to_char_b(b, i*2, -1);

			for (auto it = players.begin(); it != players.end(); ++it)
			{
				int x = it->second.s->get_pos(0);
				int y = it->second.s->get_pos(1);

				int_to_char_b(b, it->second.id*2, x);
				int_to_char_b(b, it->second.id*2 + 1, y);
			}

			int_to_char_b(b, 2, ant1.get_pos(0));
			int_to_char_b(b, 3, ant1.get_pos(1));
			b[0] = 1;

			for (auto it = players.begin(); it != players.end(); ++it)
			{
				client.sin_addr.s_addr = it->first;
				sendto(s, b, (last_id*2 + 2)*ONE_INT, 0, (struct sockaddr*)&client, sizeof(client));
			}
		}

		if(!(++frame%FRAME_INTERVAL))
		{

			bool change;

			for (auto it = players.begin(); it != players.end(); ++it)
			{
				bool sm = it->second.ttm && !it->second.s->test_collide(it->second.move_dir, speed);
				change = it->second.moving != sm;
				it->second.moving = sm;

				if (change || it->second.new_dir)
				{
					b[0] = 6;
					b[1] = it->second.id;
					b[2] = it->second.move_dir;
					b[3] = it->second.moving;
					for (auto it2 = players.begin(); it2 != players.end(); ++it2)
					{
						printf("server; sent 6 to %d, id %d, move_dir %d, moving %d\n", it2->second.id, it->second.id, it->second.move_dir, it->second.moving);
						client.sin_addr.s_addr = it2->first;
						sendto(s, b, 4, 0, (struct sockaddr*)&client, sizeof(client));
					}
				}
				if (it->second.new_dir)
				{
					printf("new dir\n");
					switch (it->second.move_dir)
					{
						case UP:
						it->second.s->change_anim(0);
						break;

						case DOWN:
						it->second.s->change_anim(1);
						break;

						case LEFT:
						it->second.s->change_anim(2);
						break;

						case RIGHT:
						it->second.s->change_anim(3);
					}
					it->second.new_dir = false;
				}

				if (it->second.moving)
				{
					it->second.s->update_anim();
					it->second.s->move(it->second.move_dir, speed);				
				}


			}

			bool prev_moving = moving;
			dir prev_move_dir = move_dir;

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

			change = (prev_moving != moving) || (prev_move_dir != move_dir);

			if (change)
			{
				b[0] = 6;
				b[1] = 1;
				b[2] = move_dir;
				b[3] = moving;
				for (auto it = players.begin(); it != players.end(); ++it)
				{
					client.sin_addr.s_addr = it->first;
					sendto(s, b, 4, 0, (struct sockaddr*)&client, sizeof(client));
				}
			}

			if (moving)
			{
				ant1.update_anim();
				ant1.move(move_dir, speed);				
			}

		}

		SDL_RenderCopy(r, background, NULL, NULL);
		ant1.render_anim();
		for (auto it = players.begin(); it != players.end(); ++it)
			it->second.s->render_anim();
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

	for (auto it = players.begin(); it != players.end(); ++it)
	{
		delete it->second.s;
	}

//	freeaddrinfo(info);
	close(s);
	WSACleanup();

	return 0;
}