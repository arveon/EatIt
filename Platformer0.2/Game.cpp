#include "Game.h"

//game update logic
void Game::Update()
{
	//check if user tried to close window by pressing X window button
	//gather the left mouse button events and process them
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
			gameOver = true;
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				LMB_DOWN = false;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				LMB_DOWN = true;
		}
		
	}

	//update time
	int elapsedTime = SDL_GetTicks() - lastUpdated;
	lastUpdated += elapsedTime;
	capFramerate(&elapsedTime);
	
	//get the keyboard state for this update
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	//check the state of the game and update appropriately
	if (game_state == SystemConstants::Menu)
	{
		//safeguard against menu not being properly initialised
		if (menu == nullptr)
		{
			std::cout << "Menu not initialised" << std::endl;
			gameOver = true;
		}

		//update the menu to check if used pressed any buttons and update buttons
		menu->Update(mouse_x, mouse_y, LMB_DOWN);

		if (menu->getType() == MenuConstants::MainMenu)
			sound_manager->stopMusic();

		//exit pause menu if escape is pressed
		if(menu->getType() == MenuConstants::PauseMenu)
			if (keyboardState[SDL_SCANCODE_ESCAPE] && !afterPause)
			{
				game_state = SystemConstants::GameGoing;
				afterPause = true;
			}
			else if (!keyboardState[SDL_SCANCODE_ESCAPE])
				afterPause = false;

		//play a button press sound if a button is pressed
		if(menu->getButtonPressed() != MenuConstants::None)
			sound_manager->buttonClick();


		//check if buttons were pressed and update game accordingly
		if (menu->getButtonPressed() == MenuConstants::StartGame && menu->getType() == MenuConstants::MainMenu)
		{
			sound_manager->startMusic();
			menu->resetButtonPressed();

			initGame();
			game_state = SystemConstants::GameGoing;
		}
		else if(menu->getButtonPressed() == MenuConstants::Exit)
		{
			//exit button in pause menu gets player to main menu resetting the game, exit button in main menu exits the game
			if (menu->getType() == MenuConstants::MainMenu)
			{
				gameOver = true;
			}
			else if (menu->getType() == MenuConstants::PauseMenu)
			{
				toMainMenu();
				menu->resetButtonPressed();
			}
		}
		else if (menu->getButtonPressed() == MenuConstants::ContinueGame)//resume game if appropriate button pressed
		{
			menu->resetButtonPressed();	
			game_state = SystemConstants::GameGoing;
		}
	}
	else if (game_state == SystemConstants::GameGoing)
	{
		if (player->isFinished())
		{
			restart();
			toMainMenu();
			game_state = SystemConstants::Menu;
			menu->setType(MenuConstants::MainMenu);
			return;
		}

		//if player is dead, restart the game
		if (!player->isObjectActive())
			restart();

		//add the enemy killed to the list of killed enemies
		if (player->killedEnemy != 0)
		{
			switch (player->getCurrentRoom())
			{
			case GameConstants::Portal_A:
				enemiesKilledOnLevel[0].push_back(player->killedEnemy);
				break;
			case GameConstants::Portal_B:
				enemiesKilledOnLevel[1].push_back(player->killedEnemy);
				break;
			case GameConstants::Portal_C:
				enemiesKilledOnLevel[2].push_back(player->killedEnemy);
				break;
			case GameConstants::Portal_D:
				enemiesKilledOnLevel[3].push_back(player->killedEnemy);
				break;
			}
			player->killedEnemy = 0;
			sound_manager->playEat();
		}

		GameConstants::ObjectType portalEntered = player->getPortal();
		//if player is in the portal, teleport to the appropriate part of level
		if (portalEntered > GameConstants::Portal_None)
		{
			std::string level;
			//depending on one of the portals, switch to the appropriate level
			switch (portalEntered)
			{
			case GameConstants::Portal_A:
				currentRoom = 0;
				level = SystemConstants::LEVEL_0;
				break;
			case GameConstants::Portal_B:
				currentRoom = 1;
				level = SystemConstants::LEVEL_1;
				break;
			case GameConstants::Portal_C:
				currentRoom = 2;
				level = SystemConstants::LEVEL_2;
				break;
			case GameConstants::Portal_D:
				currentRoom = 3;
				level = SystemConstants::LEVEL_3;
				break;
			case GameConstants::Portal_E:
				player->setCurrentRoom(GameConstants::Portal_None);
				currentRoom = 0;
				level = SystemConstants::LEVEL_0;
				curLevel++;
				restart();
				//player->resetBetweenLevels();
				//std::cout << "afterPlayerReset curRoom: " << player->getCurrentRoom() << std::endl;
				break;
			case GameConstants::Portal_F:
				currentRoom = 0;
				level = SystemConstants::LEVEL_0_0;
				player->finished();
				sound_manager->playWin();
				return;
				break;
			}
			//create a path for the next level
			std::stringstream path;
			path << SystemConstants::MAPS_FOLDER << curLevel << "/" << level;
			currentMap = path.str();


			player->resetPortal();//reset player portal status
			LoadNewLevel(portalEntered);//load the new level
		}

		//if user presses R, resets the game
		if (keyboardState[SDL_SCANCODE_R] && !afterRestart)
		{
			afterRestart = true;//needed so that game doesn't keep restarting several times as user can be holding the button
			std::stringstream path;
			path << SystemConstants::MAPS_FOLDER << curLevel << "/" << SystemConstants::LEVEL_0;
			currentMap = path.str();
			restart();
		}
		else if(!keyboardState[SDL_SCANCODE_R])
			afterRestart = false;

		//if user presses ESC, open secondary menu
		if (keyboardState[SDL_SCANCODE_ESCAPE] && !afterPause)
		{
			afterPause = true;
			game_state = SystemConstants::Menu;
			menu->setType(MenuConstants::PauseMenu);
			sound_manager->playSecondaryMenu();
		}
		else if(!keyboardState[SDL_SCANCODE_ESCAPE])
			afterPause = false;

		//update player
		player->Update(elapsedTime, keyboardState, map->getMap());
		camera->Update((int)player->getOriginX(), (int)player->getOriginY());

		//update enemies
		for (unsigned int i = 0; i < enemies->size(); i++)
		{
			enemies->at(i)->Update(elapsedTime, map->getMap());
		}

		//update other game objects
		for (unsigned int i = 0; i < gameObjects->size(); i++)
		{
			gameObjects->at(i)->Update(elapsedTime, map->getMap());
		}

		//check for player and enemy collisions
		for (unsigned int i = 0; i < enemies->size(); i++)
		{
			if (SDL_HasIntersection(player->getCollRectangle(), enemies->at(i)->getCollRectangle()) && !enemies->at(i)->isEaten())
			{
				if (player->canEat(enemies->at(i)->getEnemyType()))
				{
					enemies->at(i)->getEaten();
					player->feed(enemies->at(i)->getEnemyType(), enemies->at(i)->getID());
				}
				else
					player->takeDamage(true);
			}
		}

		//remove inactive objects
		for (unsigned int i = enemies->size() - 1; i >= 0; i--)
		{
			//need the check because using unsigned int
			if (i > enemies->size())
				break;

			if (!enemies->at(i)->isObjectActive())
			{
				Enemy* temp = enemies->at(i);
				enemies->erase(enemies->begin() + i);
				delete temp;
			}
		}

		sound_manager->playerSound(player->getState(), player->getPrevState());
		if (player->tookDamage)
		{
			sound_manager->damageTaken();
			player->tookDamage = false;
		}
	}
}
	

//draw logic
void Game::Draw()
{
	//reset screen
	SDL_RenderClear(renderer);

	//if menu is main menu, don't draw the level
	if (game_state == SystemConstants::Menu && menu->getType() == MenuConstants::MainMenu)
	{
		menu->Draw(renderer);
		SDL_RenderPresent(renderer);
		return;
	}
	//draw background
	SDL_RenderCopy(renderer, cur_bg, camera->getCameraRect(), NULL);

	//draw enemies
	for (unsigned int i = 0; i < enemies->size(); i++)
	{
		enemies->at(i)->Draw(renderer, camera->getCameraRect());
	}

	//draw other game objects
	for (unsigned int i = 0; i < gameObjects->size(); i++)
	{
		if (gameObjects->at(i)->isObjectDrawable())
			gameObjects->at(i)->Draw(renderer, camera->getCameraRect());
	}

	//draw player
	player->Draw(renderer, camera->getCameraRect());

	map->Draw(renderer, camera->getCameraRect());

	//is menu is pause menu, draw menu in a small portion of the window
	if (game_state == SystemConstants::Menu && menu->getType() == MenuConstants::PauseMenu)
	{
		menu->Draw(renderer);
	}

	SDL_RenderPresent(renderer);
}

//method will cap framerate at framerateCap
//int needs to be a pointer, because value should also change in Update method
void Game::capFramerate(int* elapsedTime)
{
	//cap framerate at 60
	int frameRate = 0;

	//if framerate is higher than 
	if (*elapsedTime < SystemConstants::TIME_PER_FRAME)
	{
		float delay = (float)SystemConstants::TIME_PER_FRAME - *elapsedTime;
			delay = ceil(delay);
		SDL_Delay((Uint32)delay);
		*elapsedTime += (int)delay;
	}
	frameRate = 1000 / *elapsedTime;
	
	/*
	//will display average framerate from last x updates
	if (counter < SystemConstants::FRAMERATE_UPDATE_TIME)
	{//if time to update not reached, add to total and increase counter
		totalFramerate += frameRate;
		counter++;
	}
	else
	{//if time reached, display framerate and reset counter and total
		std::cout << (totalFramerate / SystemConstants::FRAMERATE_UPDATE_TIME) << std::endl;
		counter = 0;
		totalFramerate = 0;
	}
	*/
	
}


//stops the game
void Game::stop()
{
	gameOver = true;
}

//game reset logic (may also be used to load other levels, as it also reloads the level
void Game::LoadNewLevel(GameConstants::ObjectType portalEntered)
{
	//logic for resetting everything
	removeGameObjects();
	initMapObjects();
	initEntities();
	//reset player
	//want to keep player between levels
	SDL_Point temp = map->getPlayerPortalSpawn(player->getCurrentRoom());
	//std::cout << "player moved from: " << player->getCurrentRoom() << " to: " << portalEntered << " at: " << temp.x / MapConstants::TILE_WIDTH << " : " << temp.y / MapConstants::TILE_HEIGHT << std::endl;
	player->setCoordinates(temp.x, temp.y);
	//check required because portal E has a different function to other portals
	if (portalEntered != GameConstants::Portal_E)
		player->setCurrentRoom(portalEntered);
	else
		player->setCurrentRoom(GameConstants::Portal_A);
	player->setActive();

	//set current background to the appropriate one
	switch (portalEntered)
	{
	case GameConstants::Portal_A:
		cur_bg = bg_textures[0];
		break;
	case GameConstants::Portal_B:
		cur_bg = bg_textures[1];
		break;
	case GameConstants::Portal_C:
		cur_bg = bg_textures[2];
		break;
	case GameConstants::Portal_D:
		cur_bg = bg_textures[3];
		break;
	}

}

//game reset logic (may also be used to load other levels, as it also reloads the level)
void Game::restart()
{
	std::stringstream newmap;
	newmap << "maps/" << curLevel << "/" << SystemConstants::LEVEL_0;
	currentMap = newmap.str();
	currentRoom = 0;
	//delete old backgrounds and load new ones
	for (int i = 0; i < 4; i++)
	{
		SDL_DestroyTexture(bg_textures[i]);
		std::stringstream path;
		path << "graphics/bg/" << curLevel << "/" << i << ".png";
		bg_textures[i] = loadTexture(path.str(), renderer);
		if (bg_textures[i] == nullptr)
			std::cout << path.str() << " load failed" << std::endl;
	}
	cur_bg = bg_textures[0];

	//reset vectors when new level entered/player died
	for (int i = 0; i < 4; i++)
		while (enemiesKilledOnLevel[i].size() > 0)
			enemiesKilledOnLevel[i].pop_back();

	delete player;
	//logic for resetting everything
	removeGameObjects();
	initMapObjects();
	initPlayer();
	initEntities();
}

//method used to remove all the game objects on the map (including the map)
void Game::removeGameObjects()
{
	//delete map, camera, all game objects and game object vector
	delete camera;
	if(gameObjects != nullptr && gameObjects->size() > 0)
		for (int i = gameObjects->size() - 1; i >= 0; i--)
		{
			GameObject* temp = gameObjects->at(i);
			gameObjects->erase(gameObjects->begin() + i);
			delete temp;
		}

	if(enemies != nullptr && enemies->size() > 0)
		for (int i = enemies->size() - 1; i > 0; i--)
		{
			Enemy* temp = enemies->at(i);
			enemies->erase(enemies->begin() + i);
			delete temp;
		}

	delete map;
}

//initialise game
Game::Game()
{
	counter = 0;
	totalFramerate = 0;
	
	afterRestart = false;
	curLevel = 1;
	currentRoom = 0;
	//setting map path
	std::stringstream path;
	path << SystemConstants::MAPS_FOLDER << curLevel << "/" << SystemConstants::LEVEL_0;
	currentMap = path.str();
	

	lastUpdated = SDL_GetTicks();
	bg_textures = new SDL_Texture*[4];
	
	//initialise enemy counters for every room on level
	enemiesKilledOnLevel = new std::vector<int>[4];
	game_state = SystemConstants::Menu;
	menu_type = MenuConstants::MainMenu;
	LMB_DOWN = false;
}

Game::~Game()
{
	//deallocate map textures
	SDL_DestroyTexture(grass_t);
	SDL_DestroyTexture(dirt_t);
	SDL_DestroyTexture(stone_t);
	SDL_DestroyTexture(brick_t);
	SDL_DestroyTexture(glass_t);
	SDL_DestroyTexture(spikes_t);
	SDL_DestroyTexture(long_grass_t);

	//deallocate other textures
	SDL_DestroyTexture(player_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(mainWindow);
	SDL_DestroyTexture(enemy_t);
	SDL_DestroyTexture(main_menu_bg);
	SDL_DestroyTexture(pause_menu_bg);
	SDL_DestroyTexture(mouse_t);

	//deallocate button textures
	for (int i = button_t->size() - 1; i > 0; i--)
	{
		SDL_Texture* temp = button_t->at(i);
		button_t->erase(button_t->begin() + i);
		SDL_DestroyTexture(temp);
	}

	//deallocate backgrounds
	for (int i = 0; i < 4; i++)
		SDL_DestroyTexture(bg_textures[i]);
	delete button_t;
	delete bg_textures;
	delete menu;

	delete sound_manager;
	//removeGameObjects();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

//game loop
void Game::run()
{
	initSystem();//provide first level to load as an argument
	//game loop
	while (!gameOver)
	{
		Update();
		Draw();
	}
}

void Game::initSystem()
{
	initSDL();//initialise SDL functions
	loadTextures();//load necessary textures
	menu = new Menu(main_menu_bg, pause_menu_bg, MenuConstants::MainMenu, button_t, mouse_t);
	sound_manager = new SoundManager();
	sound_manager->loadSounds();
}

//initialisation methods
void Game::initGame()
{
	initMapObjects();//load map & map objects
	initPlayer();
	initEntities();//initialise all the entities on the level
}

//initialises SDL components required to run the program
void Game::initSDL()
{
	//initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Couldn't init everything. SDL error: " << SDL_GetError() << std::endl;
		exit(1);
	}

	//create a window
	mainWindow = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SystemConstants::WINDOW_WIDTH, SystemConstants::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (mainWindow == nullptr)
	{
		std::cout << "Couldn't create a window. SDL error: " << SDL_GetError() << std::endl;
		exit(2);
	}

	//create a renderer for the window
	renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		std::cout << "Couldn't create a renderer. SDL error: " << SDL_GetError() << std::endl;
		exit(3);
	}
	SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0x0);//set render color
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);//set scaling mode to "nearest pixel" - no smoothing (for pixel-based sprites)

	//initialise image flags
	int img_flags = IMG_INIT_PNG;
	if (!IMG_Init(img_flags) & img_flags)
	{
		std::cout << "Couldn't initialise image flags. SDL error: " << IMG_GetError() << std::endl;
		exit(4);
	}

	//init sound
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
	{
		std::cout << "SDL mixer could not initialise. Mixer erorr: " << Mix_GetError() << std::endl;
		exit(5);
	}

	//scales the picture in the camera to screen size by changing the renderer scale
	SDL_RenderSetScale(renderer, SystemConstants::SCALING_H,SystemConstants::SCALING_V);
	SDL_ShowCursor(SDL_DISABLE);
}

//loads textures into game fields
void Game::loadTextures()
{
	//load level backgrounds
	for (int i = 0; i < 4; i++)
	{
		std::stringstream path;
		path << "graphics/bg/" << curLevel << "/" << i << ".png";
		bg_textures[i] = loadTexture(path.str(), renderer);
	}
	cur_bg = bg_textures[0];

	//load entity textures
	player_texture = loadTexture("graphics/dog_spritesheet.png", renderer);
	enemy_t = loadTexture("graphics/chicken_leg_spritesheet.png", renderer);
	cheese_t = loadTexture("graphics/cheese_spritesheet.png", renderer);
	bone_t = loadTexture("graphics/treat_spritesheet.png", renderer);
	hoover_t = loadTexture("graphics/hoover_spritesheet.png", renderer);

	//load block textures
	grass_t = loadTexture("graphics/grass.png", renderer);
	dirt_t = loadTexture("graphics/dirt.png", renderer);
	stone_t = loadTexture("graphics/stone.png", renderer);
	m_stone_t = loadTexture("graphics/mossy_stone.png", renderer);
	brick_t = loadTexture("graphics/brick.png", renderer);
	glass_t = loadTexture("graphics/glass.png", renderer);
	spikes_t = loadTexture("graphics/spikes.png", renderer);
	wood_t = loadTexture("graphics/light_wood.png", renderer);
	dark_wood_t = loadTexture("graphics/wood.png", renderer);
	
	//load decoration textures
	pot_t = loadTexture("graphics/pot.png", renderer);
	vase_t = loadTexture("graphics/vase.png", renderer);
	flower_t = loadTexture("graphics/flower.png", renderer);
	vines_t = loadTexture("graphics/vines.png", renderer);
	long_grass_t = loadTexture("graphics/temp_grass.png", renderer);
	
	//menu textures
	main_menu_bg = loadTexture("graphics/menu_bg.png", renderer);
	pause_menu_bg = loadTexture("graphics/pause_menu_bg.png", renderer);
	//menu button textures
	button_t = new std::vector<SDL_Texture*>();
	button_t->push_back(loadTexture("graphics/buttons/exit.png", renderer));
	button_t->push_back(loadTexture("graphics/buttons/start.png", renderer));
	button_t->push_back(loadTexture("graphics/buttons/continue.png", renderer));

	//pointer
	mouse_t = loadTexture("graphics/mouse.png", renderer);
}

//loads a texture and returns a pointer to it
SDL_Texture* Game::loadTexture(std::string filepath, SDL_Renderer* renderer)
{
	SDL_Texture* temp = IMG_LoadTexture(renderer, filepath.c_str());
	if (temp == nullptr)
		std::cout << "failed to load texture: " << filepath << std::endl;
	return temp;
}

//logic to load the map and initialise game objects
void Game::initMapObjects()
{
	map = new TileMap(grass_t, dirt_t, stone_t, m_stone_t, brick_t, glass_t, wood_t, dark_wood_t, spikes_t, pot_t, vase_t, long_grass_t, flower_t, vines_t, enemy_t, bone_t, cheese_t, hoover_t);
	if (!map->LoadMap(currentMap, enemiesKilledOnLevel, currentRoom))
	{
		std::cout << "Map " << currentMap << " couldn't be loaded!" << std::endl;
		SDL_Delay(10000);
		exit(10);
	}
}

//logic to load game entities (player, camera and other objects
void Game::initEntities()
{
	gameObjects = new std::vector<GameObject*>;
	std::vector<GameObject*>* temp = map->getObjectPtr();
	for (unsigned int i = 0; i < temp->size(); i++)
	{
		gameObjects->push_back(temp->at(i));
		temp->at(i)->setCollisionObjects(gameObjects);
	}

	//push one test enemy into vector
	enemies = map->getEnemies();
	for (unsigned int i = 0; i < enemies->size(); i++)
	{
		enemies->at(i)->setCollisionObjects(gameObjects);
		//std::cout << "enm: " << enemies->at(i)->getCollRectangle()->x << " : " << enemies->at(i)->getCollRectangle()->y << std::endl;
	}
	/*enemies->push_back(new Enemy(enemy_t, 0, 200, true, true, GameConstants::Enemy, gameObjects));
	enemies->push_back(new Enemy(enemy_t, 0, 400, true, true, GameConstants::Enemy, gameObjects));
	enemies->push_back(new Enemy(enemy_t, 300, 100, true, true, GameConstants::Enemy, gameObjects));*/

	//set player collision objects and the camera
	player->setCollisionObjects(gameObjects);
	camera = new Camera((int)player->getOriginX(), (int)player->getOriginY());
	
	lastUpdated = SDL_GetTicks();
}

//initialise player on heap (needed separately, because sometimes need to reset the map without resetting the player)
void Game::initPlayer()
{
	SDL_Point temp = map->getPlayerPortalSpawn(GameConstants::Portal_None);
	player = new Player(player_texture, (float)temp.x, (float)temp.y);
	player->setCurrentRoom(GameConstants::ObjectType::Portal_A);
}

//resets game when player exits to main menu. (resets the entire game progress + player)
void Game::toMainMenu()
{
	curLevel = 1;
	currentRoom = 0;
	currentMap = SystemConstants::LEVEL_0_0;

	//delete old backgrounds and load new ones
	for (int i = 0; i < 4; i++)
	{
		SDL_DestroyTexture(bg_textures[i]);
		std::stringstream path;
		path << "graphics/bg/" << curLevel << "/" << i << ".png";
		bg_textures[i] = loadTexture(path.str(), renderer);
		if (bg_textures[i] == nullptr)
			std::cout << path.str() << " load failed" << std::endl;
	}
	cur_bg = bg_textures[0];

	//reset vectors when new level entered/player died
	for (int i = 0; i < 4; i++)
		while (enemiesKilledOnLevel[i].size() > 0)
			enemiesKilledOnLevel[i].pop_back();

	delete player;
	removeGameObjects();

	menu->setType(MenuConstants::MainMenu);
}