#include "TileMap.h"

using namespace std;

//map drawing logic
void TileMap::Draw(SDL_Renderer* renderer, SDL_Rect* cameraRect)
{
	//loop through the entire map and draw every tile
	for (int i = 0; i < heightTiles; i++)
	{
		for (int j = 0; j < widthTiles; j++)
			if (tileMap[i][j] != NULL)
				tileMap[i][j]->Draw(renderer, cameraRect);
	}
}

//method loads the map with the given name
bool TileMap::LoadMap(string filepath, vector<int>* killed_enemies, int map)
{
	//default spawn coordinates (if player position not specified on the map)
	playerSpawn.x = 0;
	playerSpawn.y = 0;
	

	//open connection to the file
	ifstream file;
	file.open(filepath);

	//check if file was found
	if (!file.is_open())
		return false;

	string line;
	int line_n = 0;//needed to calculate y coordinate of every new tile added (HAS TO BE OUTSIDE THE WHILE LOOP)
	//loop through the lines in the file
	while (getline(file, line))
	{
		//loop through every character in the line
		for (unsigned int i = 0; i < line.length(); i++)
		{
			if ((signed int)i < widthTiles)//in case field in the file is wider than allowed by game
			{
				//if id is not 0, create a game object for it
				if (line[i] != '0')
				{
					int tx, ty, tempID;
					bool e_found = false;
					switch (line[i])
					{
					case '1':
						tileMap[line_n][i] = new GameObject(grass_t, i * (float)MapConstants::TILE_WIDTH, line_n * (float)MapConstants::TILE_HEIGHT, true, true, GameConstants::Map_Grass);
						break;
					case '2':
						tileMap[line_n][i] = new GameObject(dirt_t, i * (float)MapConstants::TILE_WIDTH, line_n * (float)MapConstants::TILE_HEIGHT, true, true, GameConstants::Map_Dirt);
						break;
					case '3':
						tileMap[line_n][i] = new GameObject(stone_t, i * (float)MapConstants::TILE_WIDTH, line_n * (float)MapConstants::TILE_HEIGHT, true, true, GameConstants::Map_Stone);
						break;
					case '5':
						tileMap[line_n][i] = new GameObject(m_stone_t, i * (float)MapConstants::TILE_WIDTH, line_n * (float)MapConstants::TILE_HEIGHT, true, true, GameConstants::Map_Stone);
						break;
					case '4':
						tileMap[line_n][i] = new GameObject(brick_t, i * (float)MapConstants::TILE_WIDTH, line_n *(float)MapConstants::TILE_HEIGHT, true, true, GameConstants::Map_Brick);
						break;
					case '6':
						tileMap[line_n][i] = new GameObject(glass_t, i * (float)MapConstants::TILE_WIDTH, line_n *(float)MapConstants::TILE_HEIGHT, true, true, GameConstants::Map_Brick);
						break;
					case '7':
						tileMap[line_n][i] = new GameObject(wood_t, i * (float)MapConstants::TILE_WIDTH, line_n *(float)MapConstants::TILE_HEIGHT, true, true, GameConstants::Map_Brick);
						break;
					case '8':
						tileMap[line_n][i] = new GameObject(dark_wood_t, i * (float)MapConstants::TILE_WIDTH, line_n *(float)MapConstants::TILE_HEIGHT, true, true, GameConstants::Map_Brick);
						break;
					case 'g':
						tileMap[line_n][i] = new GameObject(long_grass_t, i * (float)MapConstants::TILE_WIDTH, line_n *(float)MapConstants::TILE_HEIGHT, false, true, GameConstants::Decoration);
						break;
					case 't':
						gameObjects.push_back(new GameObject(pot_t, i * (float)MapConstants::TILE_WIDTH, line_n * (float)MapConstants::TILE_HEIGHT, false, true, GameConstants::Decoration));
						break;
					case 'v':
						gameObjects.push_back(new GameObject(vase_t, i * (float)MapConstants::TILE_WIDTH, line_n * (float)MapConstants::TILE_HEIGHT, false, true, GameConstants::Decoration));
						break;
					case 'i':
						tileMap[line_n][i] = new GameObject(vines_t, i * (float)MapConstants::TILE_WIDTH, line_n *(float)MapConstants::TILE_HEIGHT, false, true, GameConstants::Decoration);
						break;
					case 'f':
						tileMap[line_n][i] = new GameObject(flower_t, i * (float)MapConstants::TILE_WIDTH, line_n * (float)MapConstants::TILE_HEIGHT, false, true, GameConstants::Decoration);
						break;
					case 's':
						tx = i * MapConstants::TILE_WIDTH;
						//because spikes take half of the tile height, need to calculate the position so that they will be on the bottom of the tile, not at the top
						ty = (line_n * MapConstants::TILE_HEIGHT) + MapConstants::TILE_HEIGHT - MapConstants::SPIKES_HEIGHT;
						gameObjects.push_back(new GameObject(spikes_t, (float)tx, (float)ty, true, true, GameConstants::Map_Spikes));
						break;
					case 'p':
						//set player spawn positions depending on which portal the p is next to
						char portal;
						if (i > 0 && (line[i - 1] == 'a' || line[i - 1] == 'b' || line[i - 1] == 'c' || line[i - 1] == 'd'))
							portal = line[i - 1];
						else if ((signed int)i < widthTiles && (line[i + 1] == 'a' || line[i + 1] == 'b' || line[i + 1] == 'c' || line[i + 1] == 'd'))
							portal = line[i + 1];
						else
							portal = '0';
						storePlayerSpawn(i, line_n, portal);
						break;
					case 'a':
						tx = getPos(0, i);
						ty = getPos(1, line_n);
						gameObjects.push_back(new GameObject(brick_t, (float)tx, (float)ty, false, false, GameConstants::Portal_A));
						break;
					case 'b':
						tx = getPos(0, i);
						ty = getPos(1, line_n);
						gameObjects.push_back(new GameObject(brick_t, (float)tx, (float)ty, false, false, GameConstants::Portal_B));
						break;
					case 'c':
						tx = getPos(0, i);
						ty = getPos(1, line_n);
						gameObjects.push_back(new GameObject(brick_t, (float)tx, (float)ty, false, false, GameConstants::Portal_C));
						break;
					case 'd':
						tx = getPos(0, i);
						ty = getPos(1, line_n);
						gameObjects.push_back(new GameObject(brick_t, (float)tx, (float)ty, false, false, GameConstants::Portal_D));
						break;
					case 'e':
						tx = getPos(0, i);
						ty = getPos(1, line_n);
						gameObjects.push_back(new GameObject(brick_t, (float)tx, (float)ty, false, false, GameConstants::Portal_E));
						break;
					case 'k':
						gameObjects.push_back(new GameObject(brick_t, i * (float)MapConstants::TILE_WIDTH, line_n *(float)MapConstants::TILE_HEIGHT, false, false, GameConstants::Portal_F));
						break;
					case 'l':
						tempID = (line_n * i) + (line_n - i);//don't add enemy to list if it already has been killed
						for (unsigned int cn = 0; cn < killed_enemies[map].size(); cn++)
						{
							if (killed_enemies[map].at(cn) == tempID)
							{
								e_found = true;
								break;
							}
						}
						if(!e_found)//only add if such id found
							enemies.push_back(new Enemy(enemy_t, i * (float)MapConstants::TILE_WIDTH, (line_n) * (float)MapConstants::TILE_HEIGHT, true, true, EnemyConstants::ChickenLeg, &gameObjects, tempID));
						//std::cout << "enemy loaded: " << i * MapConstants::TILE_WIDTH << " : " << (line_n)* MapConstants::TILE_HEIGHT << std::endl;
						break;
					case 'n':
						tempID = (line_n * i) + (line_n - i);//same as above
						for (unsigned int cn = 0; cn < killed_enemies[map].size(); cn++)
						{
							if (killed_enemies[map].at(cn) == tempID)
							{
								e_found = true;
								break;
							}
						}
						if(!e_found)
							enemies.push_back(new Enemy(bone_t, i * (float)MapConstants::TILE_WIDTH, (line_n) * (float)MapConstants::TILE_HEIGHT, true, true, EnemyConstants::Bone, &gameObjects, tempID));
						//std::cout << "enemy loaded: " << i * MapConstants::TILE_WIDTH << " : " << (line_n)* MapConstants::TILE_HEIGHT << std::endl;
						break;
					case 'h':
						tempID = (line_n * i) + (line_n - i);//same as above
						for (unsigned int cn = 0; cn < killed_enemies[map].size(); cn++)
						{
							if (killed_enemies[map].at(cn) == tempID)
							{
								e_found = true;
								break;
							}
						}
						if (!e_found)
							enemies.push_back(new Enemy(cheese_t, i * (float)MapConstants::TILE_WIDTH, (line_n) * (float)MapConstants::TILE_HEIGHT, true, true, EnemyConstants::Cheese, &gameObjects, tempID));
						break;
					case 'r':
						tempID = (line_n * i) + (line_n - i);//same as above
						for (unsigned int cn = 0; cn < killed_enemies[map].size(); cn++)
						{
							if (killed_enemies[map].at(cn) == tempID)
							{
								e_found = true;
								break;
							}
						}
						if (!e_found)
							enemies.push_back(new Enemy(hoover_t, i * (float)MapConstants::TILE_WIDTH, (line_n) * (float)MapConstants::TILE_HEIGHT, true, true, EnemyConstants::Hoover, &gameObjects, tempID));
						break;
					}
				}
			}
			else
				break;
		}
		line_n++;
		//if more lines than allowed by size of the field
		if (line_n >= heightTiles)
			break;
	}
	file.close();//close connection to the file
	return true;
}

//stores different player spawnpoints (used to store spawnpoints near portals and default one)
void TileMap::storePlayerSpawn(int x, int y, char portal)
{
	//if default spawn not set, set it to the current portal
	if (!defset)
	{
		//cout << portal << " set as default: " << x << " : " << y << endl;
		playerSpawn.x = x * MapConstants::TILE_WIDTH;
		playerSpawn.y = y * MapConstants::TILE_HEIGHT;
	}

	switch (portal)
	{
	case 'a':
		//cout << "A set: " << x << " : " << y << endl;
		playerSpawnA.x = x * MapConstants::TILE_WIDTH;
		playerSpawnA.y = y * MapConstants::TILE_HEIGHT;
		set[0] = true;
		break;
	case 'b':
		//cout << "B set: " << x << " : " << y << endl;
		playerSpawnB.x = x * MapConstants::TILE_WIDTH;
		playerSpawnB.y = y * MapConstants::TILE_HEIGHT;
		set[1] = true;
		break;
	case 'c':
		//cout << "C set: " << x << " : " << y << endl;
		playerSpawnC.x = x * MapConstants::TILE_WIDTH;
		playerSpawnC.y = y * MapConstants::TILE_HEIGHT;
		set[2] = true;
		break;
	case 'd':
		//cout << "D set: " << x << " : " << y << endl;
		playerSpawnD.x = x * MapConstants::TILE_WIDTH;
		playerSpawnD.y = y * MapConstants::TILE_HEIGHT;
		set[3] = true;
		break;
	case '0':
		//cout << "default set: " << x << " : " << y << endl;
		playerSpawn.x = x * MapConstants::TILE_WIDTH;
		playerSpawn.y = y * MapConstants::TILE_HEIGHT;
		defset = true;
		break;
	}
	//cout << portal << " set to coords: " << x << " : " << y << endl;
}

//method calculates the position for the portal
//mode 0 - x axis, mode 1 - y axis
int TileMap::getPos(int mode, int counter)
{
	if (mode == 0)
	{
		//if on the right side of the window, place at the end of the last tile
		if (counter + 1 == widthTiles)
			return (counter * MapConstants::TILE_WIDTH + 31);
		else//if on the left side place at the start of the tile
			return (counter*MapConstants::TILE_WIDTH);
	}
	else if (mode == 1)
	{
		//if top place at the top of tile, if bottom place slightly higher than bottom of the tile
		if (counter + 1 == heightTiles)
			return (counter * MapConstants::TILE_HEIGHT + 20);
		else
			return (counter * MapConstants::TILE_HEIGHT);
	}
	else
		return 0;
}

//get the spawnpoint of where player should spawn when moving from roomLeaving to room where portal leads to
SDL_Point TileMap::getPlayerPortalSpawn(GameConstants::ObjectType roomLeaving)
{
	//cout << "leaving: " << roomLeaving << ". entering: " << portal << endl;
	if (roomLeaving == GameConstants::Portal_A && set[0])//if entering from A and A is set, return A portal spawnpoint
		return playerSpawnA;
	else if (roomLeaving == GameConstants::Portal_B && set[1])//if entering from B and B is set, return B portal spawnpoint
		return playerSpawnB;
	else if (roomLeaving == GameConstants::Portal_C && set[2])//if entering from C and C is set, return C portal spawnpoint
		return playerSpawnC;
	else if (roomLeaving == GameConstants::Portal_D && set[3])
		return playerSpawnD;
	else
		return playerSpawn;//if none of the above (e.g. the portal is not set) return default spawnpoint
}

//init the tilemap object
TileMap::TileMap(SDL_Texture* grass_t, SDL_Texture* dirt_t, SDL_Texture* stone_t, SDL_Texture* m_stone_t, SDL_Texture* brick_t, SDL_Texture* glass_t, SDL_Texture* wood_t, SDL_Texture* dark_wood_t, SDL_Texture* spikes_t,
	SDL_Texture* pot_t, SDL_Texture* vase_t, SDL_Texture* long_grass_t, SDL_Texture* flower_t, SDL_Texture* vines_t, 
	SDL_Texture* enm, SDL_Texture* bone_t, SDL_Texture* cheese_t, SDL_Texture* hoover_t)
{
	//set textures
	this->grass_t = grass_t;
	this->dirt_t = dirt_t;
	this->stone_t = stone_t;
	this->m_stone_t = m_stone_t;
	this->brick_t = brick_t;
	this->glass_t = glass_t;
	this->spikes_t = spikes_t;
	this->long_grass_t = long_grass_t;
	this->enemy_t = enm;
	this->bone_t = bone_t;
	this->cheese_t = cheese_t;
	this->pot_t = pot_t;
	this->vase_t = vase_t;
	this->flower_t = flower_t;
	this->vines_t = vines_t;
	this->wood_t = wood_t;
	this->dark_wood_t = dark_wood_t;
	this->hoover_t = hoover_t;


	//allowed number of tiles on x and y axis;
	heightTiles = GameConstants::LEVEL_HEIGHT / MapConstants::TILE_HEIGHT;
	widthTiles = GameConstants::LEVEL_WIDTH / MapConstants::TILE_WIDTH;

	//initialise the array
	tileMap = new GameObject**[heightTiles];
	for (int i = 0; i < heightTiles; i++)
	{
		tileMap[i] = new GameObject*[widthTiles];
		for (int j = 0; j < widthTiles; j++)
			tileMap[i][j] = NULL;
	}

	defset = false;
	for (int i = 0; i < 4; i++)
		set[i] = false;
}


TileMap::~TileMap()
{
	//deallocate all objects
	for (int i = 0; i < heightTiles; i++)
	{
		for (int j = 0; j < widthTiles; j++)
		{
			if (tileMap[i][j] != NULL)
				delete (tileMap[i][j]);
		}
	}
	delete tileMap;
}

//initial collision detection (doesn't work)
/*
//checks if given rectangle collides with one of the tiles
vector<GameObject*>* TileMap::CollidesWithTile(SDL_Rect collisionRect)
{
int wlocation = collisionRect.x/MapConstants::TILE_WIDTH;
int hlocation = collisionRect.y/MapConstants::TILE_HEIGHT;

bool coll = false;
vector<GameObject*>* collisionObjs= new vector<GameObject*>;

for (int i = 0; i < heightTiles; i++)
{
int dy = wlocation - i;
for (int j = 0; j < widthTiles; j++)
{
int dx = wlocation - j;
if (dx < -3)
break;

if (dx > 3)
continue;

if (tileMap[i][j] != NULL)
{
SDL_Rect* trect = tileMap[i][j]->getCollRectangle();
coll = SDL_HasIntersection(trect, &collisionRect);

collisionObjs->push_back(tileMap[i][j]);
}
}
}
return collisionObjs;
}
*/