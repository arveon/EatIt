#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

//loads a texture and returns a pointer to it
SDL_Texture* HelperFunctions::loadTexture(std::string filepath, SDL_Renderer* renderer)
{
	SDL_Texture* temp = IMG_LoadTexture(renderer, filepath.c_str());
	if (temp == nullptr)
		std::cout << "failed to load texture: " << filepath << std::endl;
	return temp;
	////load image into surface
	//SDL_Surface* temp = IMG_Load(filepath.c_str());
	//if (temp == nullptr)
	//{
	//	std::cout << "Couldn't load texture. SDL error: " << IMG_GetError() << std::endl;
	//	exit(5);
	//}

	////convert surface into texture
	//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp);
	//if (texture == nullptr)
	//{
	//	std::cout << "Couldn't create texture from surface. SDL error: " << SDL_GetError() << std::endl;
	//	exit(6);
	//}
	//return texture;
}