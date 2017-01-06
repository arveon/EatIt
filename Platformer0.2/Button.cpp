#include "Button.h"



Button::Button(int x, int y, SDL_Texture* button_t, MenuConstants::ButtonType type)
{
	this->type = type;
	this->button_t = button_t;
	collision_rect = new SDL_Rect();
	collision_rect->x = x;
	collision_rect->y = y;
	collision_rect->w = MenuConstants::MAIN_MENU_BUTTON_WIDTH;
	collision_rect->h = MenuConstants::MAIN_MENU_BUTTON_HEIGHT;

	//source rectangle
	cur_frame.x = 0;
	cur_frame.y = 0;
	cur_frame.w = MenuConstants::BUTTON_TEXTURE_WIDTH;
	cur_frame.h = MenuConstants::BUTTON_TEXTURE_HEIGHT;

	buttonPressed = false;
}


Button::~Button()
{
}

bool Button::Update(int mouse_x, int mouse_y, bool LMB_Down)
{
	//represents whether the button was actually clicked (value returned)
	bool buttonClicked = false;

	//checking if mouse coordinates are within the button rectangle bounds
	if (mouse_x > collision_rect->x && mouse_x < collision_rect->x + collision_rect->w)
		if (mouse_y > collision_rect->y && mouse_y < collision_rect->y + collision_rect->h)
		{
			//set the current frame to the second spritesheet frame (onHover)
			cur_frame.x = MenuConstants::BUTTON_TEXTURE_WIDTH + 2;

			//if button is pressed, set pressed to true
			if (LMB_Down)
				buttonPressed = true;
			else if (buttonPressed && !LMB_Down)
			{//if button was pressed and now mouse button is released, player clicked the button
				buttonClicked = true;
				buttonPressed = false;//reset pressed value as it is now not pressed
			}
			else
				buttonPressed = false;

			//if button is pressed, switch to the third frame
			if (buttonPressed)
				cur_frame.x = 2 * (MenuConstants::BUTTON_TEXTURE_WIDTH + 2);
		}
		else
		{//mouse isn't over the button, reset everything
			cur_frame.x = 0;
			buttonPressed = false;
		}
	else
	{//mouse isn't over the button, reset everything
		cur_frame.x = 0;
		buttonPressed = false;
	}

	return buttonClicked;
}

void Button::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, button_t, &cur_frame, collision_rect);
}