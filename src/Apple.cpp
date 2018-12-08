#include <iostream>
#include <random>
#include <ctime>

#include "Apple.h"
#include "Texture.h"

#define screen_width 800
#define screen_height 600

Apple::Apple(int scl) {
	grid_scale = scl;
	position = { grid_scale*random(5, 35), grid_scale*random(5, 25) };
}

void Apple::update(SDL_Renderer* renderer, SDL_Point snake_position, vector<SDL_Point> tail_positions, vector<SDL_Point> obstacle_positions) {
	render(position.x, position.y, renderer);
	
	if (position.x == snake_position.x && position.y == snake_position.y) {
		is_eaten(tail_positions, obstacle_positions);
	}
}

// function is called when snake's and apple's positions match
void Apple::is_eaten(vector<SDL_Point> tail_positions, vector<SDL_Point> obstacle_positions) {
	bool colliding = true;
	SDL_Point new_position;

	// we search for empty position until we find one
	while (colliding) {
		colliding = false;
		new_position = free_positions[random(0, free_positions.size())];
		for (int i = 0; i < tail_positions.size(); i++) {
			if (new_position.x == tail_positions[i].x && new_position.y == tail_positions[i].y) {
				colliding = true;
				break;
			}
		}
	}

	// we assign randomly picked empty position to apple's position
	position = new_position;
}

// create array of positions where obstacle positions are excluded
void Apple::create_free_grid(vector<SDL_Point> obstacle_positions) {
	free_positions.clear();
	for (int i = 0; i < screen_width; i += grid_scale) {
		for (int j = 0; j < screen_height; j += grid_scale) {
			SDL_Point pos = { i, j };
			if (!check_if_in(pos, obstacle_positions)) {
				free_positions.push_back(pos);
			}
		}
	}
}

// checks if the randomly picked position matches with one of the obstacle positions
bool Apple::check_if_in(SDL_Point pos, vector<SDL_Point> obstacle_positions) {
	int count = 0;
	for (int i = 0; i < obstacle_positions.size(); i++) {
		if (pos.x != obstacle_positions[i].x || pos.y != obstacle_positions[i].y) {
			count++;
		}
	}

	if (count == obstacle_positions.size()) {
		return false;
	}
	else {
		return true;
	}
}

SDL_Point Apple::get_position() {
	return position;
}

void Apple::restart() {
	position = { grid_scale*random(5, 35), grid_scale*random(5, 25) };
}

int Apple::random(int min, int max) {
	std::random_device random_device; // create object for seeding
	std::mt19937 engine{ random_device() }; // create engine and seed it
	std::uniform_int_distribution<> dist(min, max); // create distribution for integers with [min; max] range
	return dist(engine);
}