//#pragma once
#include <SDL_image.h>
#include <iostream>


class Indicators
{
private:
	SDL_Texture* lt;
	SDL_Rect* drawRectangle;
	int numLives;
public:
	Indicators(SDL_Texture*);
	void Draw(SDL_Renderer*);
	void Update(int);
	~Indicators();
};

