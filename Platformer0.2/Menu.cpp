#include "Menu.h"

Menu::Menu(SDL_Texture* menu_bg, SDL_Texture* pause_menu_bg, MenuConstants::MenuType type, std::vector<SDL_Texture*>* button_t, SDL_Texture* mouse_t)
{
	this->menu_bg = menu_bg;
	this->pause_menu_bg = pause_menu_bg;
	this->button_ts = button_t;
	this->type = type;
	drawRect = new SDL_Rect();
	InitMenu();
	mouse = new Mouse(mouse_t);
}

Menu::~Menu()
{
	delete drawRect;
}

void Menu::Draw(SDL_Renderer* renderer)
{	
	//depending on the type of menu, either draw it on full screen or on a portion
	if(type == MenuConstants::MainMenu)
		SDL_RenderCopy(renderer, menu_bg, NULL, NULL);
	else
		SDL_RenderCopy(renderer, pause_menu_bg, NULL, drawRect);

	//draw all the buttons
	for (int i = 0; i < buttons.size(); i++)
		buttons.at(i)->Draw(renderer);

	//draw mouse
	mouse->Draw(renderer);
}

void Menu::Update(int mouse_x, int mouse_y, bool LMB_Down)
{
	//required because the drawing scale is dependent on the screen size/camera size ratio and need to account for that
	//for the coordinate system to work properly
	mouse_x /= SystemConstants::SCALING_H;
	mouse_y /= SystemConstants::SCALING_V;
	mouse->Update(mouse_x, mouse_y, LMB_Down);

	//update all the buttons
	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons.at(i)->Update(mouse_x, mouse_y, LMB_Down))
			button_pressed = buttons.at(i)->getType();
	}
}

void Menu::InitMenu()
{
	//remove all the buttons from vector
	for (int i = buttons.size() - 1; i >= 0; i--)
	{
		Button* temp = buttons.at(i);
		buttons.erase(buttons.begin() + i);
		delete temp;
	}

	//depending on the type of menu set drawRect and buttons accordingly
	switch (type)
	{
	case MenuConstants::MainMenu:
		drawRect->x = 0;
		drawRect->y = 0;
		drawRect->w = SystemConstants::WINDOW_WIDTH;
		drawRect->h = SystemConstants::WINDOW_HEIGHT;
		buttons.push_back(new Button(MenuConstants::MAIN_MENU_START_X, MenuConstants::MAIN_MENU_START_Y, button_ts->at(1), MenuConstants::StartGame));
		buttons.push_back(new Button(MenuConstants::MAIN_MENU_EXIT_X, MenuConstants::MAIN_MENU_EXIT_Y, button_ts->at(0), MenuConstants::Exit));
		break;
	case MenuConstants::PauseMenu:
		drawRect->w = MenuConstants::PAUSE_MENU_WINDOW_WIDTH;
		drawRect->h = MenuConstants::PAUSE_MENU_WINDOW_HEIGHT;
		drawRect->x = (SystemConstants::WINDOW_WIDTH / 3) - (drawRect->w / 2);
		drawRect->y = (SystemConstants::WINDOW_HEIGHT / 3) - (drawRect->h / 2);
		buttons.push_back(new Button(drawRect->x + MenuConstants::PAUSE_MENU_RESUME_X_OFF, drawRect->y + MenuConstants::PAUSE_MENU_RESUME_Y_OFF, button_ts->at(2), MenuConstants::ContinueGame));
		buttons.push_back(new Button(drawRect->x + MenuConstants::PAUSE_MENU_EXIT_X_OFF, drawRect->y + MenuConstants::PAUSE_MENU_EXIT_Y_OFF, button_ts->at(0), MenuConstants::Exit));
		break;
	}
}