#include <SDL.h>
#include <SDL_image.h>

#define HEIGHT 600
#define WIDTH 800

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* w = SDL_CreateWindow("haaaa!!!!!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
	IMG_Init(IMG_INIT_PNG);

	SDL_Surface* img = IMG_Load("test.png");
	SDL_Texture* background = SDL_CreateTextureFromSurface(r, img);
	SDL_FreeSurface(img);

	bool game_over = false;
	SDL_Event ia;
	const Uint8 *state;

	while (!game_over)
	{

		SDL_PollEvent(&ia);

		state = SDL_GetKeyboardState(NULL);

		if (ia.type == SDL_QUIT)
		{

			SDL_DestroyTexture(background);
			SDL_DestroyRenderer(r);
			SDL_DestroyWindow(w);
			w = NULL;
			r = NULL;
			IMG_Quit();
			SDL_Quit();
			return 0;
		}


		SDL_RenderClear(r);


		SDL_RenderCopy(r, background, NULL, NULL);
		
		SDL_RenderPresent(r);


	}
	return 0;
}