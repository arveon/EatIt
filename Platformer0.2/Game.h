#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <sstream>
#include <cassert>
#include <exception>

#include "Constants.h"
#include "GameObject.h"
#include "Player.h"
#include "Camera.h"
#include "TileMap.h"
#include "Enemy.h"
#include "Menu.h"
#include "SoundManager.h"

class Game
{
private:
	//drawing support
	SDL_Window* mainWindow = nullptr;
	SDL_Renderer* renderer = nullptr;

	SystemConstants::GameState game_state;
	MenuConstants::MenuType menu_type;
	Menu* menu;
	SoundManager* sound_manager = nullptr;

	//general textures (bg, player, enemy etc)
	SDL_Texture* main_menu_bg = nullptr;
	SDL_Texture* pause_menu_bg = nullptr;
	SDL_Texture** bg_textures = nullptr;
	SDL_Texture* cur_bg = nullptr;

	SDL_Texture* mouse_t = nullptr;

	SDL_Texture* player_texture = nullptr;

	//textures
	SDL_Texture* grass_t;
	SDL_Texture* dirt_t;
	SDL_Texture* stone_t;
	SDL_Texture* m_stone_t;
	SDL_Texture* brick_t;
	SDL_Texture* glass_t;
	SDL_Texture* wood_t;
	SDL_Texture* dark_wood_t;

	SDL_Texture* pot_t;
	SDL_Texture* vase_t;
	SDL_Texture* flower_t;
	SDL_Texture* spikes_t;
	SDL_Texture* vines_t;
	SDL_Texture* long_grass_t;

	SDL_Texture* enemy_t;
	SDL_Texture* bone_t;
	SDL_Texture* cheese_t;
	SDL_Texture* hoover_t;

	
	
	std::vector<SDL_Texture*>* button_t;
	
	//time of the last update
	Uint32 lastUpdated = 0;

	//is game over or not
	bool gameOver;
	
	//game objects
	Player* player;
	Camera* camera;
	TileMap* map;
	std::vector<GameObject*>* gameObjects;
	std::vector<Enemy*>* enemies;

	std::vector<int>* enemiesKilledOnLevel;
	
	//needed to prevent restarting multiple times while holding the key
	bool afterRestart;
	bool afterPause;
	bool LMB_DOWN;

	int currentRoom;
	std::string currentMap;
	int curLevel;

	//required to displayed framerate every 10 updates
	int counter;
	int totalFramerate;

	void Update();
	void Draw();
	void toMainMenu();
	void initGame();
	void initSystem();
	void initSDL();
	void loadTextures();
	void initMapObjects();
	void initEntities();
	void initPlayer();
	void removeGameObjects();
	SDL_Texture* loadTexture(std::string filepath, SDL_Renderer* renderer);
	void LoadNewLevel(GameConstants::ObjectType portalEntered);
	void restart();
	void stop();

	void capFramerate(int* elapsedTime);
public:
	Game();
	~Game();
	void run();
};

