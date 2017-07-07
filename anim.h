#ifndef ANIM_H
#define ANIM_H


#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

struct Vec2d
{
	int x;
	int y;
};

class Animation
{
	private:

		Vec2d S;

		SDL_Rect * frames;
		SDL_Texture* some_txtr;

		unsigned int cframe = 0;
		unsigned int NUM_FRAMES;
	public:
		Animation(Vec2d s, unsigned int nf, std::string file, SDL_Renderer* rend)
		: S(s)
		, NUM_FRAMES(nf)
		, frames(new SDL_Rect[nf])
		{
			SDL_Surface* im = SDL_CreateRGBSurface(0,s.x*nf,s.y,32,0,0,0,0);
			SDL_Rect re = {0, 0, s.x*nf, s.y};
			SDL_BlitScaled(IMG_Load(file.c_str()), NULL, im, &re);
			SDL_SetColorKey(im, SDL_TRUE, SDL_MapRGB(im->format, 0xFF, 0xFF, 0xFF));
			some_txtr = SDL_CreateTextureFromSurface(rend, im);

			for (int i = 0; i < nf; ++i)
			{
				frames[i] = {i*s.x, 0, s.x, s.y};
			}

			SDL_FreeSurface(im);
		}
		~Animation()
		{
			printf("dieeeeeeeee");
			SDL_DestroyTexture(some_txtr);
			delete frames;
			some_txtr = NULL;
		}

		Vec2d get_size(){return S;}
		unsigned int get_nf(){return NUM_FRAMES;}
		void update(){cframe = ((cframe + 1) == NUM_FRAMES) ? 0 : (cframe + 1);}
		void revert(){cframe = (cframe == 0) ? (NUM_FRAMES - 1) : (cframe - 1);}
		void reset(){cframe = 0;}
		void render_frame(SDL_Renderer* rend, int pos_y, int pos_x)
		{
			SDL_Rect final = {pos_x, pos_y, S.x, S.y};
			SDL_RenderCopy(rend, some_txtr, frames+cframe, &final);
		}

};

#endif