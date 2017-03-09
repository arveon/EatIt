#include "Mouse.h"

//everything is simple enough and self-explanatory
//cur_frame is not currently used as there is no spritesheet for the mouse

Mouse::Mouse(SDL_Texture* mouse_t)
{
	this->mouse_t = mouse_t;
	
	draw_rect.x = 0;
	draw_rect.y = 0;
	draw_rect.w = 24;
	draw_rect.h = 24;

	cur_frame.x = 0;
	cur_frame.y = 0;
	cur_frame.w = 16;
	cur_frame.h = 16;
}

Mouse::~Mouse()
{
	SDL_DestroyTexture(mouse_t);
}


void Mouse::Update(int x, int y, bool lmb_down)
{
	draw_rect.x = x;
	draw_rect.y = y;
}

void Mouse::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, mouse_t, &cur_frame, &draw_rect);
}