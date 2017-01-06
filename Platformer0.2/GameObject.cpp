#include "GameObject.h"

#include <iostream>

void GameObject::Update(Uint32 elapsedTime, GameObject*** map)
{
	//only update if it's not a map object (map objects start at index 10)
	if (type < 10)
	{
		//not resetting Y because it isn't dependent on the movement direction, it depends on player's position and gravity
		velocityX = 0;

		//change object velocity if it should be moving
		if (movementDir != 0)
		{
			//only check the X axis, as it's the only one game objects control
			switch (movementDir)
			{
			case GameConstants::Direction::Left:
				velocityX = -obj_speed;
				break;
			case GameConstants::Direction::Right:
				velocityX = obj_speed;
				break;
			}
		}
		//needed to correctly draw sprites
		if(movementDir != GameConstants::None)
			facingDir = movementDir;

		if (!noGravityMode)
		{
			//apply gravity
			if (!onGround)
			{
				velocityY += (float)(GameConstants::GRAVITY*elapsedTime);
			}
			else
				velocityY = 0;//if on ground, gravity doesn't apply
			onGround = false;//reset after every update (is set in resolveCollisionsX if collision exists)	
		}
		
	/*	if (fallingDistance / MapConstants::TILE_HEIGHT >= 2)
		{
			fallingDamageFlag = true;
			fallingDistance = 0;
		}*/

		//move object on the Y axis and resolve collisions
		if (y + velocityY <= GameConstants::LEVEL_HEIGHT && y + collisionRect->h + velocityY >= 0)
			y += velocityY * elapsedTime;
		resolveCollisionsX(map, 1);

		//move object on the X axis and resolve collisions
		if (x + velocityX <= GameConstants::LEVEL_WIDTH && x + collisionRect->w + velocityX >= 0)
			x += velocityX * elapsedTime;
		resolveCollisionsX(map, 0);

		//clamp object to level size
		if (x < 0)
			x = 0;
		else if (x + collisionRect->w >= GameConstants::LEVEL_WIDTH)
			x = (float)GameConstants::LEVEL_WIDTH - collisionRect->w;
		if (y < 0)
			y = 0;
		else if (y + collisionRect->h >= GameConstants::LEVEL_HEIGHT)
			y = (float)GameConstants::LEVEL_HEIGHT - collisionRect->h;

		//update the collision position
		collisionRect->x = (int)x;
		collisionRect->y = (int)y;
	}
}

//checks for collisions and updates position
void GameObject::resolveCollisionsX(GameObject*** map, int axis)
{
	//map coordinates of the object
	int cx = (int)x / MapConstants::TILE_WIDTH;
	int cy = (int)y / MapConstants::TILE_HEIGHT;

	//loop through all tiles where collision is possible
	for (int i = cy; i < (y + collisionRect->h) / MapConstants::TILE_HEIGHT; i++)
	{
		if (i > MapConstants::TILES_Y)
			break;

		for (int j = cx; j < (x+collisionRect->w) / MapConstants::TILE_WIDTH; j++)
		{
			if (i >= GameConstants::LEVEL_HEIGHT / MapConstants::TILE_HEIGHT)
				throw "player out of level bounds";

			//if object takes up a non-empty tile
			if (i >= 0 && j >= 0 && i < MapConstants::TILES_Y && j < MapConstants::TILES_X && map[i][j] != NULL)
			{
				if (map[i][j]->isObjSolid())
				{
					//if (velocityX > 0 && axis == 0 && map[i][j]->type!=GameConstants::Map_Spikes)//collision on the right
					if (velocityX > 0 && axis == 0)//collision on the right
					{
						x = (float)j * MapConstants::TILE_WIDTH - collisionRect->w;//set the object position to the previous tile
						velocityX = 0;
					}
					//if (velocityX < 0 && axis == 0 && map[i][j]->type != GameConstants::Map_Spikes) //collision on the left
					if (velocityX < 0 && axis == 0) //collision on the left
					{
						x = (float)j * MapConstants::TILE_WIDTH + MapConstants::TILE_WIDTH;//set the object position to the next tile
						velocityX = 0;
					}

					//if moving down (means landed on the ground or spikes)
					if (velocityY > 0 && axis == 1)
					{
						y = (float)i * MapConstants::TILE_HEIGHT - collisionRect->h;//set player position to the tile above
						velocityY = 0;
						onGround = true;
					}

					//if moving up (hit something with the head)
					if (velocityY < 0 && axis == 1)
					{
						y = (float)i * MapConstants::TILE_HEIGHT + MapConstants::TILE_HEIGHT;//set player position to the tile below
						velocityY = 0;
					}
				}
			}
		}
	}
}

//logic to draw the object
void GameObject::Draw(SDL_Renderer* renderer, SDL_Rect* cameraRect)
{
	if (isDrawable)
	{
		//update draw rect
		drawRect->x = (int)x;
		drawRect->y = (int)y;

		//set draw rect position relative to the camera centre
		drawRect->x -= cameraRect->x;
		drawRect->y -= cameraRect->y;

		SDL_Rect crop = { 0, 0,  curFrame->w, curFrame->h};
		//SDL_Point center = {crop.x + (crop.w/2, crop.y + (crop.h/2))};
		//if not outside camera, draw the object 
		if (drawRect->x + drawRect->w >= 0 && drawRect->x <= GameConstants::CAMERA_WIDTH && drawRect->y + drawRect->h >= 0 && drawRect->y <= GameConstants::CAMERA_HEIGHT)
			switch (facingDir)
			{
			case GameConstants::Left:
				SDL_RenderCopyEx(renderer, texture, curFrame, drawRect, -180, NULL, SDL_FLIP_VERTICAL);
				break;
			case GameConstants::Right:
				SDL_RenderCopyEx(renderer, texture, curFrame, drawRect, 0, NULL, SDL_FLIP_NONE);
				break;
			}
			
	}
}

//initialise the game object
GameObject::GameObject(SDL_Texture* texture, float x, float y, bool isSolid, bool isDrawable, GameConstants::ObjectType type)
{
	//object properties
	this->isSolid = isSolid;
	this->isDrawable = isDrawable;
	this->type = type;
	isActive = true;

	this->texture = texture;

	//object positioning
	this->x = x;
	this->y = y;
	drawRect = new SDL_Rect{ (int)x, (int)y, MapConstants::TILE_WIDTH, MapConstants::TILE_HEIGHT };
	collisionRect = new SDL_Rect{ (int)x, (int)y, MapConstants::TILE_WIDTH, MapConstants::TILE_HEIGHT };

	//required because spikes take half the tile
	if (type == GameConstants::Map_Spikes)
	{
		drawRect->h = MapConstants::SPIKES_HEIGHT;
		collisionRect->h = MapConstants::SPIKES_HEIGHT;
	}
	else if (type > GameConstants::Portal_None)
	{//portals on sides of the screen are only 1 pixel wide
		if (x + MapConstants::TILE_WIDTH == GameConstants::LEVEL_WIDTH || x == 0)
		{
			drawRect->w = 1;
			collisionRect->w = 1;
		}
		else if (y + MapConstants::TILE_HEIGHT == GameConstants::LEVEL_HEIGHT || y == 0)
		{//portals on top and bottom of the screen are 4 pixels tall
			drawRect->h = 4;
			collisionRect->h = 4;
		}
	}

	//movement
	movementDir = GameConstants::Direction::None;
	onGround = false;
	obj_speed = GameConstants::BASE_OBJECT_VELOCITY;
	facingDir = GameConstants::Right;
	curFrame = new SDL_Rect();
	curFrame->x = 0;
	curFrame->y = 0;
	curFrame->w = SystemConstants::TEXTURE_SIZE;
	curFrame->h = SystemConstants::TEXTURE_SIZE;

	//std::cout << "obj: " << x << " : " << y << std::endl;
	noGravityMode = false;
	//jumpXPenaltyMultiplier = GameConstants::BASE_JUMP_PENALTY_MULTIPLIER;
}

//destroy the object
GameObject::~GameObject()
{
	delete curFrame;
	delete collisionRect;
	delete drawRect;
}
