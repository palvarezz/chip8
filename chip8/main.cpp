#define _CRT_SECURE_NO_DEPRECATE
#include "sdl.h"
#include "stdio.h"
#include "chip8.h"

void initG(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture * texture);
void sKeys(chip8 &mychip);
void Destroy();

void SDL_UPDATE(SDL_Texture* texture, void* buffer, int pitch, SDL_Renderer* renderer);


int main(int argc, char* args[]) {
	chip8 mychip;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* texture = NULL;
	int pitch = sizeof(mychip.gfx[0]) * 64;
	//setup renderer
	//initG(window,renderer,texture);
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL Failed to Init : %s", SDL_GetError());
		exit(0);
	}
	if ((window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN)) == NULL) {
		printf("Failed to create window: %s", SDL_GetError());
		exit(0);
	}
	if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(0);
	}

	if ((texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32)) == NULL) {
		printf("Failed to create texture: %s\n", SDL_GetError());
		exit(0);
	}
	 
	//intialize chip8
	  mychip.initialize();
	//load game
	 mychip.loadGame("IBM Logo.ch8");
	 
	bool quit = false;
	SDL_Event e;

	while (!quit) {
		//set inputs;
		//sKeys(mychip);
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			SDL_Delay(5);
			mychip.emulateCycle();
			if (mychip.drawflag)
			{
				pitch = sizeof(mychip.gfx[0]) * 64;
				//SDL_UPDATE(texture, mychip.gfx, pitch, renderer);
				if (SDL_UpdateTexture(texture, NULL, mychip.gfx, pitch) != 0) {
					printf("Failed to update texture: %s\n", SDL_GetError());
					exit(0);
				}
				if (SDL_RenderClear(renderer) != 0) {
					printf("Failed to render clear: %s\n", SDL_GetError());
					exit(0);
				}
				if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0) {
					printf("Failed to render copy: %s\n", SDL_GetError());
					exit(0);
				}
				SDL_RenderPresent(renderer);
				mychip.drawflag = 0;
			}
			

		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_Quit();
	return 0;
}
void SDL_UPDATE(SDL_Texture* texture, void* buffer, int pitch, SDL_Renderer* renderer) {

	if (SDL_UpdateTexture(texture, NULL, buffer, pitch) != 0) {
		printf("Failed to update texture: %s\n", SDL_GetError());
		exit(0);
	}
	if (SDL_RenderClear(renderer) != 0) {
		printf("Failed to render clear: %s\n", SDL_GetError());
		exit(0);
	}
	if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0) {
		printf("Failed to render copy: %s\n", SDL_GetError());
		exit(0);
	}
	SDL_RenderPresent(renderer);

}
void initG(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture * texture) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL Failed to Init : %s", SDL_GetError());
		exit(0);
	}
	if ((window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN)) == NULL) {
		printf("Failed to create window: %s", SDL_GetError());
		exit(0);
	}
	if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(0);
	 }
	
	if ((texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32)) == NULL) {
		printf("Failed to create texture: %s\n", SDL_GetError());
		exit(0);
	}

}

void sKeys(chip8 &mychip) {

	bool quit = false;
	SDL_Event e;
	int EXIT = 1;
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		//User presses a key
		else if (e.type == SDL_KEYDOWN)
		{
			//Select surfaces based on key press
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			{
				EXIT = 1;
			} break;

			case SDLK_x:
			{
				mychip.setKeys(0x000B,1);
				
			} break;

			case SDLK_1:
			{
				mychip.setKeys(0x0001,1);
			} break;

			case SDLK_2:
			{
				mychip.setKeys(0x0002,1);
			} break;

			case SDLK_3:
			{
				mychip.setKeys(0x0003,1);
			} break;

			case SDLK_q:
			{
				mychip.setKeys(0x0004,1);
			} break;

			case SDLK_w:
			{
				mychip.setKeys(0x0005, 1);
			} break;

			case SDLK_e:
			{
				mychip.setKeys(0x0006, 1);
			} break;

			case SDLK_a:
			{
				mychip.setKeys(0x0007, 1);
			} break;

			case SDLK_s:
			{
				mychip.setKeys(0x0008, 1);
			} break;

			case SDLK_d:
			{
				mychip.setKeys(0x0009, 1);
			} break;

			case SDLK_z:
			{
				mychip.setKeys(0x000A, 1);
			} break;

			case SDLK_c:
			{
				mychip.setKeys(0x000C, 1);
			} break;

			case SDLK_4:
			{
				mychip.setKeys(0x000D, 1);
			} break;

			case SDLK_r:
			{
				mychip.setKeys(0x000E, 1);;
			} break;

			case SDLK_f:
			{
				mychip.setKeys(0x000F, 1);
			} break;

			case SDLK_v:
			{
				mychip.setKeys(0x0000, 1);
			} break;
			}
			break;
			}
		else if(e.type == SDL_KEYUP){
			//Select surfaces based on key press
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			{
				EXIT = 0;
			} break;

			case SDLK_x:
			{
				mychip.setKeys(0x000B, 0);

			} break;

			case SDLK_1:
			{
				mychip.setKeys(0x0001, 0);
			} break;

			case SDLK_2:
			{
				mychip.setKeys(0x0002, 0);
			} break;

			case SDLK_3:
			{
				mychip.setKeys(0x0003, 0);
			} break;

			case SDLK_q:
			{
				mychip.setKeys(0x0004, 0);
			} break;

			case SDLK_w:
			{
				mychip.setKeys(0x0005, 0);
			} break;

			case SDLK_e:
			{
				mychip.setKeys(0x0006, 0);
			} break;

			case SDLK_a:
			{
				mychip.setKeys(0x0007, 0);
			} break;

			case SDLK_s:
			{
				mychip.setKeys(0x0008, 0);
			} break;

			case SDLK_d:
			{
				mychip.setKeys(0x0009, 0);
			} break;

			case SDLK_z:
			{
				mychip.setKeys(0x000A, 0);
			} break;

			case SDLK_c:
			{
				mychip.setKeys(0x000C, 0);
			} break;

			case SDLK_4:
			{
				mychip.setKeys(0x000D, 0);
			} break;

			case SDLK_r:
			{
				mychip.setKeys(0x000E, 0);
			} break;

			case SDLK_f:
			{
				mychip.setKeys(0x000F, 0);
			} break;

			case SDLK_v:
			{
				mychip.setKeys(0x0000, 0);
			} break;
			}
			break;
		}
		}
}
