#pragma once

#include <SDL3/SDL.h>
#include <cstdint>

class Display
{
	public:

		Display(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
		~Display();
		void Update(void const *buffer, int pitch);
		bool ProcessInput(uint8_t *keys);

	private:

		SDL_Window* window{};
		SDL_Renderer* renderer{};
		SDL_Texture* texture{};
		int textureWidth;
    		int textureHeight;
    		uint8_t* rgbBuffer;


};
