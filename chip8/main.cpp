#include "sdl.h"
#include "stdio.h"
#include "chip8.h"
#include "stdint.h"

void sKeys(chip8 &mychip);

int main(int argc, char* args[]) {
	chip8 mychip = chip8();
	
	
	SDL_Texture* texture = NULL;
	int w = 640;
	int h = 320;

	//setup renderer
	SDL_Window* window = NULL;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL Failed to Init : %s", SDL_GetError());
		exit(0);
	}
	if ((window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN)) == NULL) {
		printf("Failed to create window: %s", SDL_GetError());
		exit(0);
	}
	SDL_Renderer* renderer = NULL;
	if ((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(0);	
	}
	SDL_RenderSetLogicalSize(renderer, w, h);
	if ((texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32)) == NULL) {
		printf("Failed to create texture: %s\n", SDL_GetError());
		exit(0);
	}
	uint32_t pixels[2048];
	//intialize chip8
	 mychip.initialize();
	//load game
	 mychip.loadGame("Airplane.ch8");
	 while (true) {
		 mychip.emulateCycle();
		 SDL_Event ev;
		 while (SDL_PollEvent(&ev)) {
			 if (ev.type == SDL_QUIT) {
				 exit(0);
			 }
		 }
		 if (mychip.drawflag)
		 {
			 mychip.drawflag = 0;
			 
			 for (int i = 0; i < 2048; ++i)
			 {
				 uint8_t pixel = mychip.gfx[i];
				 pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
				 
			 }
			 if (SDL_UpdateTexture(texture, NULL, pixels,   64 * sizeof(uint32_t)) != 0) {
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
		 SDL_Delay(2);
	 }
	
	/*while (true) {
		mychip.emulateCycle();
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT)
				exit(0);
			}
		if (mychip.drawflag) {
			mychip.drawflag = false;
			for (int i = 0; i < 2048; ++i)
			{
				uint8_t pixel = mychip.gfx[i];
				pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
			}
			if (SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(Uint32)) != 0) {
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
			std::this_thread::sleep_for(std::chrono::microseconds(2000));
		}
	}*/
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_Quit();
	return 0;
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
