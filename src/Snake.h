#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>

// #pragma once
#include "Texture.h"

class Snake : public Texture {
	private:
		SDL_Point position;
		int grid_scale;
		int velocity_x; 
		int velocity_y; 
		SDL_Point final_velocity;

		vector<SDL_Point> tail_positions;
	public:
		Snake(int grid_scale, SDL_Renderer* renderer);

		void change_direction(SDL_Point final_velocity);

		bool check_collision(SDL_Point apple_position, vector<SDL_Point> obstacle_positions);

		void move(SDL_Renderer* renderer, SDL_Point apple_position, SDL_Point portal_orange, SDL_Point portal_blue, Mix_Chunk* apple_sound);

		void check_portals(SDL_Point portal_orange, SDL_Point portal_blue);

		void handle_tail(SDL_Renderer* renderer);

		void check_if_has_eaten(SDL_Point apple_position, Mix_Chunk* apple_sound);

		void wrap();

		void restart();

		vector<SDL_Point> get_tail_positions();

		SDL_Point get_position();

		void add_tail();

		int random(int min, int max);

};


