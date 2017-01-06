#include <SDL.h>
#include <vector>
#include <iostream>

#include "Button.h"
#include "Constants.h"
#include "Mouse.h"

class Menu
{
private:
	//textures and rectangles
	SDL_Texture* menu_bg;
	SDL_Texture* pause_menu_bg;
	SDL_Rect* drawRect;

	//main or pause
	MenuConstants::MenuType type;

	//mouse that appears while in menu
	Mouse* mouse;

	std::vector<Button*> buttons;
	std::vector<SDL_Texture*>* button_ts;
	MenuConstants::ButtonType button_pressed;//if any button was pressed this is set to the one that was pressed
	
	//depending on the current type of menu, set the buttons, drawRectangle, texture appropriately
	void InitMenu();
public:
	Menu(SDL_Texture* menu_bg, SDL_Texture* pause_menu_bg, MenuConstants::MenuType type, std::vector<SDL_Texture*>* button_t, SDL_Texture* mouse_t);
	~Menu();
	void Update(int, int, bool);
	void Draw(SDL_Renderer* renderer);

	void setType(MenuConstants::MenuType newType) { type = newType; InitMenu(); }
	void resetButtonPressed() { button_pressed = MenuConstants::None; }
	
	MenuConstants::ButtonType getButtonPressed() { return button_pressed; }
	MenuConstants::MenuType getType() { return type; }
	
};

