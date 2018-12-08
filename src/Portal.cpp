#include <random>
#include <ctime>

#include "Portal.h"

#define grid_scale 20

Portal::Portal(SDL_Point snake_position, SDL_Point apple_position, int min, int max) {
	position = { grid_scale*random(min, min), grid_scale*random(min, min) };
}

void Portal::update(SDL_Renderer* renderer) {
	render(position.x, position.y, renderer);
}

SDL_Point Portal::get_position() {
	return position;
}

int Portal::random(int min, int max) {
	default_random_engine generator;
	generator.seed((unsigned int)time(NULL) + 1);

	uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}