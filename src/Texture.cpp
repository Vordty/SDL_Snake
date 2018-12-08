#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

#include "Texture.h"

using namespace std;

Texture::Texture() {
	texture = nullptr;
	width = 0;
	height = 0;
}

Texture::~Texture() {
	free();
}

bool Texture::load_from_file(string path, SDL_Renderer* renderer) {
	free();

	SDL_Texture* new_texture = nullptr;
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	if (loaded_surface == nullptr) {
		cout << "error while loading the surface from IMG" << endl;
	}
	else {
		new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
		if (new_texture == nullptr) {
			cout << "error while creating texture from surface in texture class" << endl;
		}
		else {
			if (width == 0 && height == 0) {
				 width = loaded_surface->w;
				 height = loaded_surface->h;
			}
		}

		SDL_FreeSurface(loaded_surface);
	}

	texture = new_texture;
	return texture != nullptr;
}

void Texture::render(int x, int y, SDL_Renderer* renderer) {
	SDL_Rect rect = {x, y, width, height};
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Texture::free() {
	SDL_DestroyTexture(texture);
	texture = nullptr;

	width = 0;
	height = 0;
}

int Texture::get_width() {
	return width;
}

int Texture::get_height() {
	return height;
}

void Texture::set_texture(SDL_Texture* _texture) {
	texture = _texture;
}

SDL_Texture* Texture::get_texture() {
	return texture;
}