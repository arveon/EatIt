#ifndef _TILE_MAP
#define _TILE_MAP

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <vector>
#include <SDL.h>

#include "GameObject.h"
#include "Enemy.h"

class TileMap
{
private:
	GameObject*** tileMap;//map 2d array
	int widthTiles;//number of columns of tiles
	int heightTiles;//number of rows of tiles

	//tile textures
	SDL_Texture* grass_t;
	SDL_Texture* dirt_t;
	SDL_Texture* stone_t;
	SDL_Texture* m_stone_t;
	SDL_Texture* brick_t;
	SDL_Texture* glass_t;
	SDL_Texture* spikes_t;
	SDL_Texture* pot_t;
	SDL_Texture* vase_t;
	SDL_Texture* flower_t;
	SDL_Texture* long_grass_t;
	SDL_Texture* vines_t;
	SDL_Texture* wood_t;
	SDL_Texture* dark_wood_t;

	SDL_Texture* enemy_t;
	SDL_Texture* bone_t;
	SDL_Texture* cheese_t;
	SDL_Texture* hoover_t;

	//loaded objects (and enemies) that aren't map tiles (spikes won't be considered map tiles)
	std::vector<GameObject*> gameObjects;
	std::vector<Enemy*> enemies;

	//player spawning coordinates
	bool defset;
	bool set[4];
	SDL_Point playerSpawn;
	SDL_Point playerSpawnA;
	SDL_Point playerSpawnB;
	SDL_Point playerSpawnC;
	SDL_Point playerSpawnD;

	int getPos(int mode, int counter);
	void storePlayerSpawn(int x, int y, char prevchar);
public:
	TileMap(SDL_Texture* grass_t, SDL_Texture* dirt_t, SDL_Texture* stone_t, SDL_Texture* m_stone_t, SDL_Texture* brick_t, SDL_Texture* glass_t, SDL_Texture* wood_t, SDL_Texture* dark_wood_t, SDL_Texture* spikes_t,
		SDL_Texture* pot_t, SDL_Texture* vase_t, SDL_Texture* temp_grass, SDL_Texture* flower_t, SDL_Texture* vines_t, 
		SDL_Texture* enemy_t, SDL_Texture* bone_t, SDL_Texture* cheese_t, SDL_Texture* hoover_t);
	~TileMap();

	bool LoadMap(std::string filepath, std::vector<int>* killed_enemies, int map);
	void Draw(SDL_Renderer* renderer, SDL_Rect* cameraRect);
	GameObject*** getMap() { return tileMap; }
	std::vector<GameObject*>* getObjectPtr() { return &gameObjects; }
	std::vector<Enemy*>* getEnemies() { return &enemies; }
	void killEnemy() { int i = 1; }

	//access to player spawn coordinates
	SDL_Point getPlayerDefSpawn() { return playerSpawn; }
	/*int getPlayerSpawnX() { return playerX; }
	int getPlayerSpawnY() { return playerY; }*/

	SDL_Point getPlayerPortalSpawn(GameConstants::ObjectType roomLeaving);
};
#endif