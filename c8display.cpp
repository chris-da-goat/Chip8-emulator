#include "c8display.hpp"
#include <SDL3/SDL.h>


Display::Display(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight){
	
	
	SDL_Init( SDL_INIT_VIDEO );
        window = SDL_CreateWindow( title, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE );
        renderer = SDL_CreateRenderer(window, nullptr);	
    	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
    
    	this->textureWidth = textureWidth;
    	this->textureHeight = textureHeight;
    
    	rgbBuffer = new uint8_t[textureWidth * textureHeight * 3]; // 3 bytes per pixel (RGB)

}


Display::~Display() {
SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	delete[] rgbBuffer;
	SDL_Quit();
}


void Display::Update(void const* buffer, int pitch)
{

    const uint8_t* src = static_cast<const uint8_t*>(buffer);
    uint8_t* dst = rgbBuffer;
    
    for (int y = 0; y < textureHeight; y++) {
        for (int x = 0; x < textureWidth; x++) {
            int byteIndex = x / 8;
            int bitIndex = 7 - (x % 8); // MSB first
            int pixel = (src[byteIndex] >> bitIndex) & 1;
            
            // Set RGB values (white or black)
            dst[0] = pixel * 255; // R
            dst[1] = pixel * 255; // G
            dst[2] = pixel * 255; // B
            
            dst += 3;
        }
        src += pitch; // Move to next line in source buffer
    }

	SDL_UpdateTexture(texture, nullptr, rgbBuffer, textureWidth * 3);
	SDL_RenderClear(renderer);;
	SDL_RenderTexture(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}


bool Display::ProcessInput(uint8_t* keys)
{
	bool quit = false;

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_EVENT_QUIT:
			{
				quit = true;
			} break;

			case SDL_EVENT_KEY_DOWN:
			{
				switch (event.key.scancode)
				{
					case SDL_SCANCODE_ESCAPE:
					{
						quit = true;
					} break;

					case SDL_SCANCODE_X:
					{
						keys[0] = 1;
					} break;

					case SDL_SCANCODE_1:
					{
						keys[1] = 1;
					} break;

					case SDL_SCANCODE_2:
					{
						keys[2] = 1;
					} break;

					case SDL_SCANCODE_3:
					{
						keys[3] = 1;
					} break;

					case SDL_SCANCODE_Q:
					{
						keys[4] = 1;
					} break;

					case SDL_SCANCODE_W:
					{
						keys[5] = 1;
					} break;

					case SDL_SCANCODE_E:
					{
						keys[6] = 1;
					} break;

					case SDL_SCANCODE_A:
					{
						keys[7] = 1;
					} break;

					case SDL_SCANCODE_S:
					{
						keys[8] = 1;
					} break;

					case SDL_SCANCODE_D:
					{
						keys[9] = 1;
					} break;

					case SDL_SCANCODE_Z:
					{
						keys[0xA] = 1;
					} break;

					case SDL_SCANCODE_C:
					{
						keys[0xB] = 1;
					} break;

					case SDL_SCANCODE_4:
					{
						keys[0xC] = 1;
					} break;

					case SDL_SCANCODE_R:
					{
						keys[0xD] = 1;
					} break;

					case SDL_SCANCODE_F:
					{
						keys[0xE] = 1;
					} break;

					case SDL_SCANCODE_V:
					{
						keys[0xF] = 1;
					} break;
					
					default:
						break;

				}
			} break;

			case SDL_EVENT_KEY_UP:
			{
				switch (event.key.scancode)
				{
					case SDL_SCANCODE_X:
					{
						keys[0] = 0;
					} break;

					case SDL_SCANCODE_1:
					{
						keys[1] = 0;
					} break;

					case SDL_SCANCODE_2:
					{
						keys[2] = 0;
					} break;

					case SDL_SCANCODE_3:
					{
						keys[3] = 0;
					} break;

					case SDL_SCANCODE_Q:
					{
						keys[4] = 0;
					} break;

					case SDL_SCANCODE_W:
					{
						keys[5] = 0;
					} break;

					case SDL_SCANCODE_E:
					{
						keys[6] = 0;
					} break;

					case SDL_SCANCODE_A:
					{
						keys[7] = 0;
					} break;

					case SDL_SCANCODE_S:
					{
						keys[8] = 0;
					} break;

					case SDL_SCANCODE_D:
					{
						keys[9] = 0;
					} break;

					case SDL_SCANCODE_Z:
					{
						keys[0xA] = 0;
					} break;

					case SDL_SCANCODE_C:
					{
						keys[0xB] = 0;
					} break;

					case SDL_SCANCODE_4:
					{
						keys[0xC] = 0;
					} break;

					case SDL_SCANCODE_R:
					{
						keys[0xD] = 0;
					} break;

					case SDL_SCANCODE_F:
					{
						keys[0xE] = 0;
					} break;

					case SDL_SCANCODE_V:
					{
						keys[0xF] = 0;
					} break;

					default:
						break;
				}
			} break;
		}
	}

	return quit;
}

