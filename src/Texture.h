#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

using namespace std;

#pragma once

class Texture {
	protected:
		SDL_Texture* texture;
		int width;
		int height;
	public:
		Texture();

		~Texture();

		bool load_from_file(string path, SDL_Renderer* renderer);

		void set_texture(SDL_Texture* _texture);

		SDL_Texture* get_texture();

		void render(int x, int y, SDL_Renderer* renderer);

		void free();

		int get_width();

		int get_height();
};