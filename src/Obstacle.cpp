#include <random>
#include <ctime>
#include <vector>

#include "Obstacle.h"

#define grid_scale 20

Obstacle::Obstacle(int n, SDL_Point apple_position, SDL_Point portal_orange, SDL_Point portal_blue) {
	obstacle_count = n;
	instantiate(apple_position, portal_orange, portal_blue);
}

void Obstacle::instantiate(SDL_Point apple_position, SDL_Point portal_orange, SDL_Point portal_blue) {
	for (int i = 0; i < obstacle_count; i++) {
		SDL_Point point = { grid_scale*random(1, 39), grid_scale*random(1, 29) };
		if ((point.x != apple_position.x || point.y != apple_position.y) && (point.x != portal_blue.x || point.y != portal_blue.y) && (point.x != portal_orange.x || point.y != portal_orange.y)) {
			obstacle_positions.push_back(point);
		}
	}
}

void Obstacle::update(SDL_Renderer* renderer) {
	for (int i = 0; i < obstacle_positions.size(); i++) {
		render(obstacle_positions[i].x, obstacle_positions[i].y, renderer);
	}
}

void Obstacle::restart(SDL_Point apple_position, SDL_Point portal_orange, SDL_Point portal_blue) {
	obstacle_positions.clear();
	instantiate(apple_position, portal_orange, portal_blue);
}

vector<SDL_Point> Obstacle::get_positions() {
	return obstacle_positions;
}

int Obstacle::random(int min, int max) {
	std::random_device random_device; 
	std::mt19937 engine{ random_device() }; 
	std::uniform_int_distribution<> dist(min, max);
	return dist(engine);
}