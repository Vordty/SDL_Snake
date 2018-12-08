#pragma once
#include "Texture.h"

class Portal : public Texture {
	private:
		SDL_Point position;
	public:
		Portal(SDL_Point snake_position, SDL_Point apple_position, int min, int max);

		void update(SDL_Renderer* renderer);

		SDL_Point get_position();

		int random(int min, int max);
};
