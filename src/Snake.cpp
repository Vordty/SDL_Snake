#include <iostream>
#include <random>
#include <ctime>
#include <vector>

#include "Snake.h"
#include "Texture.h"

using namespace std;

Snake::Snake(int scl, SDL_Renderer* renderer){
	grid_scale = scl;
	position = { grid_scale*random(5, 30), grid_scale*random(5, 20) };

	width = grid_scale;
	height = grid_scale;

	velocity_x = 0;
	velocity_y = 0;

	tail_positions.push_back(position);

}

void Snake::change_direction(SDL_Point final_velocity) {
	velocity_x = final_velocity.x;
	velocity_y = final_velocity.y;
}

// this function checks if snake has touched its body or the obstacles
// if this returns true we restart the game in the main loop
bool Snake::check_collision(SDL_Point apple_position, vector<SDL_Point> obstacle_positions) {
	for (int i = 0; i < tail_positions.size() - 1; i++) {
		if (position.x == tail_positions[i].x && position.y == tail_positions[i].y) {
			if (position.x == apple_position.x && position.y == apple_position.y) {
				continue;
			}

			return true;
		}
	}

	for (int j = 0; j < obstacle_positions.size(); j++) {
		if (position.x == obstacle_positions[j].x && position.y == obstacle_positions[j].y) {
			return true;
		}
	}

	return false;
}

void Snake::move(SDL_Renderer* renderer, SDL_Point apple_position, SDL_Point portal_orange, SDL_Point portal_blue, Mix_Chunk* apple_sound) {
	position.x += velocity_x;
	position.y += velocity_y;

	wrap();

	check_portals(portal_orange, portal_blue);

	check_if_has_eaten(apple_position, apple_sound);

	handle_tail(renderer);

	render(position.x, position.y, renderer);
}

// checking if snake is touching the portal and which one.
void Snake::check_portals(SDL_Point portal_orange, SDL_Point portal_blue) {
	// if true we re-position snake's head on the other portal
	if (position.x == portal_orange.x && position.y == portal_orange.y) {
		position = { portal_blue.x, portal_blue.y };
	}
	else if ((position.x == portal_blue.x && position.y == portal_blue.y)) {
		position = { portal_orange.x, portal_orange.y };
	}
}

void Snake::handle_tail(SDL_Renderer* renderer) {
	// this function is called when snake has moved
	// so we need to add new head position at the end ofo tail array and throw away the first element which is the last part of the tail
	tail_positions.push_back(position);
	tail_positions.erase(tail_positions.begin());
	for (int i = 0; i < tail_positions.size(); i++) {
		render(tail_positions[i].x, tail_positions[i].y, renderer);
	}
}

// checking if snake has eaten the apple
void Snake::check_if_has_eaten(SDL_Point apple_position, Mix_Chunk* apple_sound) {
	if (position.x == apple_position.x && position.y == apple_position.y) {
		Mix_PlayChannel(-1, apple_sound, 0);

		// if positions match we increment the tail
		tail_positions.push_back(position);
	}
}

// no boundaries for snake on the screen
void Snake::wrap() {
	if (position.x >= 800) {
		position.x = 0;
	}
	else if (position.x < 0) {
		position.x = 800;
	}
	else if (position.y >= 600) {
		position.y = 0;
	}
	else if (position.y < 0) {
		position.y = 600;
	}
}

void Snake::restart() {
	// choosing position randomly
	position = { grid_scale*random(5, 30), grid_scale*random(5, 20) };

	// reseting the direction
	velocity_x = 0;
	velocity_y = 0;

	// deleting tails from the array
	tail_positions.clear();

	// pushing coordinates of snake's head in array
	tail_positions.push_back(position);

}

vector<SDL_Point> Snake::get_tail_positions() {
	return tail_positions;
}

SDL_Point Snake::get_position() {
	return position;
}

void Snake::add_tail() {
	tail_positions.push_back(position);
}

int Snake::random(int min, int max) {
	std::random_device random_device; 
	std::mt19937 engine{ random_device() }; 
	std::uniform_int_distribution<> dist(min, max);
	return dist(engine);
}