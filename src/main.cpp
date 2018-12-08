#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>

#include "Texture.h"
#include "Snake.h"
#include "Apple.h"
#include "Obstacle.h"
#include "Portal.h"

#define screen_width 800
#define screen_height 600
#define scl 20
#define fps 12

using namespace std;

bool init();
bool load_media();
void exit();
void draw_grid(int grid_scale);

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

Texture main_menu_texture;
Texture end_menu_texture;

Snake snake(20, renderer);
Apple apple(20);
Portal portal_orange(snake.get_position(), apple.get_position(), 5, 10);
Portal portal_blue(snake.get_position(), apple.get_position(), 25, 35);
Obstacle obstacles(30, apple.get_position(), portal_orange.get_position(), portal_blue.get_position());

Mix_Chunk* apple_sound = nullptr;

int main(int argc, char* argv[]) {
	init();
	load_media();

	Uint32 starting_tick;

	// create array of positions where obstacle positions are excluded
	apple.create_free_grid(obstacles.get_positions());

	// main game loop boolean
	bool running = true;
	// storing events that we recieve
	SDL_Event e;

	// booleans for handling menus
	bool main_menu = true;
	bool end_menu = false;

	// variable to change direction of the snake
	SDL_Point final_velocity = { 0, 0 };
	// limiting key presses
	bool key_pressed = false;

	while (running) {

		starting_tick = SDL_GetTicks();

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
				break;
			}
			
			// changing snake's direciton
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_a && !key_pressed) {
					if (final_velocity.x != 1 * scl) {
						final_velocity.x = -1 * scl;
						final_velocity.y = 0;
						key_pressed = true;
					}
				}
				else if (e.key.keysym.sym == SDLK_d && !key_pressed) {
					if (final_velocity.x != -1 * scl) {
						final_velocity.x = 1 * scl;
						final_velocity.y = 0;
						key_pressed = true;
					}
				}
				else if (e.key.keysym.sym == SDLK_w && !key_pressed) {
					if (final_velocity.y != 1 * scl) {
						final_velocity.x = 0;
						final_velocity.y = -1 * scl;
						key_pressed = true;
					}
				}
				else if (e.key.keysym.sym == SDLK_s && !key_pressed) {
					if (final_velocity.y != -1 * scl) {
						final_velocity.x = 0;
						final_velocity.y = 1 * scl;
						key_pressed = true;
					}
				}
				else if (e.key.keysym.sym == SDLK_j && !key_pressed) {
					apple.is_eaten(snake.get_tail_positions(), obstacles.get_positions());
					key_pressed = true;
				}
				else if (e.key.keysym.sym == SDLK_k && !key_pressed) {
					snake.add_tail();
					key_pressed = true;
				}
			}
		}

		// displaying main menu
		if (main_menu) {
			main_menu_texture.render(0, 0, renderer);
			SDL_RenderPresent(renderer);
			while (main_menu) {
				if (SDL_WaitEvent(&e)) {
					if (e.type == SDL_KEYDOWN) {
						main_menu = false;
						break;
					}
				}
			}
		}
		

		// rendering
		
		SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
		SDL_RenderClear(renderer);

		draw_grid(20);

		snake.change_direction(final_velocity);
		key_pressed = false;

		snake.move(renderer, apple.get_position(), portal_orange.get_position(), portal_blue.get_position(), apple_sound);
		if (snake.check_collision(apple.get_position(), obstacles.get_positions())) {
			SDL_Delay(500);

			// disable event handling
			key_pressed = true;

			// displaying end menu
			end_menu = true;
			if (end_menu) {
				end_menu_texture.render(0, 0, renderer);
				SDL_RenderPresent(renderer);
				SDL_Delay(2000);
				while (end_menu) {
					if (SDL_WaitEvent(&e)) {
						if (e.type == SDL_KEYDOWN) {
							main_menu = false;
							break;
						}
					}
				}
			}

			// clear the screen
			SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
			SDL_RenderClear(renderer);

			// reset snake's velocity
			final_velocity = { 0, 0 };
			// reset snake's properties
			snake.restart();

			// reset apple's properties
			apple.restart();

			obstacles.restart(apple.get_position(), portal_orange.get_position(), portal_blue.get_position());
			apple.create_free_grid(obstacles.get_positions());
		}

		apple.update(renderer, snake.get_position(), snake.get_tail_positions(), obstacles.get_positions());
		
		portal_blue.update(renderer);
		portal_orange.update(renderer);

		obstacles.update(renderer);

		SDL_RenderPresent(renderer);
		
		// caping the framerate
		if (1000 / fps > SDL_GetTicks() - starting_tick) {
			SDL_Delay(1000 / fps - (SDL_GetTicks() - starting_tick));
		}
	}

	exit();
	return 0;
}

bool init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		cout << "error at init start" << endl;
		success = false;
	}
	else {
		window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			screen_width, screen_height, SDL_WINDOW_RESIZABLE);
		if (window == nullptr) {
			cout << "error while creating the window" << endl;
			success = false;
		}
		else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == nullptr) {
				cout << "error while creating the renderer" << endl;
				success = false;
			}
			else {
				if (!IMG_Init(IMG_INIT_PNG)) {
					cout << "error while init-ing IMG_PNG" << endl;
					success = false;
				}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					cout << "error while init-ing audio" << endl;
					success = false;
				}
			}
		}
	}

	return success;
}

bool load_media() {
	bool success = true;

	if (!main_menu_texture.load_from_file("./snake_assets/main_menu.png", renderer)) {
		cout << "error while loading main menu" << endl;
		success = false;
	}
	if (!snake.load_from_file("./snake_assets/snake.png", renderer)) {
		cout << "error while loading snake head" << endl;
		success = false;
	}
	if (!apple.load_from_file("./snake_assets/apple.png", renderer)) {
		cout << "error while loading apple" << endl;
		success = false;
	}
	if (!obstacles.load_from_file("./snake_assets/obstacle.png", renderer)) {
		cout << "error while loading obstacle" << endl;
		success = false;
	}
	if (!portal_orange.load_from_file("./snake_assets/portal1.png", renderer)) {
		cout << "error while loading portal1" << endl;
		success = false;
	}
	if (!portal_blue.load_from_file("./snake_assets/portal2.png", renderer)) {
		cout << "error while loading portal2" << endl;
		success = false;
	}

	apple_sound = Mix_LoadWAV("./snake_assets/hmmm.wav");
	if (apple_sound == nullptr) {
		cout << "error while loading move sound" << endl;
		success = false;
	}
	if (!end_menu_texture.load_from_file("./snake_assets/end_menu.png", renderer)) {
		cout << "error while loading end menu" << endl;
		success = false;
	}

	return success;
}

void draw_grid(int grid_scale) {
	for (int i = 0; i < screen_width; i += grid_scale) {
		SDL_SetRenderDrawColor(renderer, 7, 7, 7, 255);
		SDL_RenderDrawLine(renderer, i, 0, i, screen_height);
	}
	for (int j = 0; j < screen_height; j += grid_scale) {
		SDL_SetRenderDrawColor(renderer, 7, 7, 7, 255);
		SDL_RenderDrawLine(renderer, 0, j, screen_width, j);
	}
}

void exit() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;

	IMG_Quit();
	SDL_Quit();
}

