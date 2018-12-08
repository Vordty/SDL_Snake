#include <vector>

#pragma once
#include "Texture.h"

class Obstacle : public Texture {
	private:
		int obstacle_count;
		vector<SDL_Point> obstacle_positions;
	public:
		Obstacle(int n, SDL_Point apple_position, SDL_Point portal_orange, SDL_Point portal_blue);

		void instantiate(SDL_Point apple_position, SDL_Point portal_orange, SDL_Point portal_blue);

		void update(SDL_Renderer* renderer);

		void restart(SDL_Point apple_position, SDL_Point portal_orange, SDL_Point portal_blue);

		vector<SDL_Point> get_positions();

		int random(int min, int max);
};

