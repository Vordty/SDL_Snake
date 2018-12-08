#pragma once
#include "Texture.h"

class Apple : public Texture {
	private:
		SDL_Point position;
		int grid_scale;

		vector<SDL_Point> free_positions;
	public:
		Apple(int scl);

		void update(SDL_Renderer* renderer, SDL_Point snake_position, vector<SDL_Point> tail_positions, vector<SDL_Point> obstacle_positions);

		void is_eaten(vector<SDL_Point> tail_positions, vector<SDL_Point> obstacle_positions);

		void create_free_grid(vector<SDL_Point> obstacle_positions);

		bool check_if_in(SDL_Point pos, vector<SDL_Point> obstacle_positions);

		SDL_Point get_position();

		void restart();

		int random(int min, int max);

};

