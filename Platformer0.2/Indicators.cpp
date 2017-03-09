#include "Indicators.h"

Indicators::Indicators(SDL_Texture* lives_texture)
{
	lt = lives_texture;
	drawRectangle = new SDL_Rect();
	drawRectangle->w = 20;
	drawRectangle->h = 20;
}

void Indicators::Draw(SDL_Renderer* renderer)
{
	int counter = 0;
	int line = 0, column = 0;
	drawRectangle->x = 30;
	drawRectangle->y = 20;
	while (counter < numLives)
	{
		drawRectangle->y = (line * 25) + 10;
		drawRectangle->x = (column * 25) + 20;

		column++;
		if (column == 5)
		{
			line++;
			column = 0;
		}

		SDL_RenderCopy(renderer, lt, NULL, drawRectangle);
		counter++;
	}
}

void Indicators::Update(int livecount)
{
	numLives = livecount;
}



Indicators::~Indicators()
{
	SDL_DestroyTexture(lt);
	delete drawRectangle;
}