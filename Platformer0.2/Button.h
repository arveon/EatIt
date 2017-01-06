#include <SDL.h>
#include <iostream>

#include "Constants.h"

class Button
{
private:
	SDL_Rect* collision_rect;
	SDL_Texture* button_t;

	SDL_Rect cur_frame;

	MenuConstants::ButtonType type;//stores the type this button represents
	bool buttonPressed;
public:
	Button(int x, int y, SDL_Texture* button_t, MenuConstants::ButtonType type);
	~Button();
	bool Update(int mouse_x, int mouse_y, bool LMB_Down);
	void Draw(SDL_Renderer* main_renderer);

	MenuConstants::ButtonType getType() { return type; }
};

