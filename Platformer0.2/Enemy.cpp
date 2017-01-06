#include "Enemy.h"

void Enemy::Update(Uint32 elapsedTime, GameObject*** map)
{
	//if player is eaten, make it fly up and deactivate it after certain number of ms
	if (eaten && elapsedDeathTime >= totalDeathTime)
	{
		isActive = false;
	}
	else if(eaten)
	{
		elapsedDeathTime += elapsedTime;
		velocityY = -0.05f;
		obj_speed /= 1.02f;
	}

	//while still looking around
	if (lookAround && elapsedLookingTime >= EnemyConstants::TOTAL_STANDING_TIME)
	{
		movementDir = next_dir;//set movement direction to previously saved next dir
		next_dir = GameConstants::None;//reset next dir, just in case
		lookAround = false;
		elapsedLookingTime = 0;
	}
	else if (lookAround)
		elapsedLookingTime += elapsedTime;


	//the x and y coordinates (in numbers of tiles) of the enemy 
	int ptx = (int)x / MapConstants::TILE_WIDTH;
	int pty = (int)y / MapConstants::TILE_HEIGHT;

	//check collision with the game objects that enemy can collide with
	for (unsigned int i = 0; i < collisionObjs->size(); i++)
	{
		//only need to check if an object is spikes (every other object will be handled differently)
		if (collisionObjs->at(i)->getType() == GameConstants::Map_Spikes)
		{
			//get the x and y coordinates (in numbers of tiles) of the spikes
			int stx = collisionObjs->at(i)->getCollRectangle()->x / MapConstants::TILE_WIDTH;
			int sty = collisionObjs->at(i)->getCollRectangle()->y / MapConstants::TILE_HEIGHT;

			//the enemy needs to jump only if the spikes are within two tile range in the direction of enemy movement (and spikes Y is the same as enemy Y)
			if ((((ptx + 1 == stx) && movementDir == GameConstants::Right)
				|| ((ptx == stx) && movementDir == GameConstants::Left)
				) && pty == sty)
			{
				jump();
			}
		}
	}

	//enemy need to change direction if reaches the edge of the level
	if (((ptx + 1) > (MapConstants::TILES_X - 1) && movementDir == GameConstants::Right)
		|| (x == 0 && movementDir == GameConstants::Left))
	{
		changeDirection();
	}

	//will change direction once meets any sort of drop (if next tile along x doesn't have ground to support enemy e.g. end of platform reached)
	/*if ((map[pty + 1][ptx + 1] == NULL || (map[pty + 1][ptx - 1] == NULL) && (int)((x - 5) / MapConstants::TILE_WIDTH) == (ptx - 1)) && onGround && movementDir != GameConstants::None)
	{*/
	if (map[pty + 1][ptx - 1] == NULL && (x - 5) / MapConstants::TILE_WIDTH < ptx && onGround && movementDir == GameConstants::Left)
		changeDirection();
	else if (map[pty + 1][ptx + 1] == NULL && onGround && movementDir == GameConstants::Right)
		changeDirection();
	else
	{
		//need to change direction only if there is an objects within two tiles when moving right (accounting for the size of enemy as well)
		if (map[pty][ptx + 1] != NULL && movementDir == GameConstants::Right)
		{
			//needed, because it doesn't treat the tiles that are outside map (nonexistent) as nulls and thus jumps at the end of the level as well
			if (ptx + 1 < MapConstants::TILES_X && map[pty][ptx + 1]->isObjSolid())
					changeDirection();
			else if (ptx + 2 > MapConstants::TILES_X || ptx - 1 < 0)
					changeDirection();
		}
		else if (map[pty][ptx - 1] != NULL && movementDir == GameConstants::Left)
		{
			//same as above
			if (ptx > 0 && map[pty][ptx - 1]->isObjSolid() && (int)((x - 5) / MapConstants::TILE_WIDTH) == (ptx - 1))//last equasion is true when enemy is on the leftmost edge of the tile
				changeDirection();
		}
	}

	GameObject::Update(elapsedTime, map);

	setDrawFrame(elapsedTime);

	//std::cout << x << " : " << y << std::endl;
}

void Enemy::Draw(SDL_Renderer* renderer, SDL_Rect* cameraRect)
{
	if(e_type == EnemyConstants::Hoover)
		y -= 16;

	GameObject::Draw(renderer, cameraRect);

	if (e_type == EnemyConstants::Hoover)
		y += 16;
}

void Enemy::jump()
{
	//only let enemy jump if it's on ground
	if (onGround)
	{
		velocityY = -2 * GameConstants::BASE_OBJECT_VELOCITY;
		y -= 1;//need that to get him off the ground
		onGround = false;
		curState = PlayerConstants::Jumping;
	}
	//std::cout << info << std::endl;
}

void Enemy::changeDirection()
{
	lookAround = true;
	//save next direction
	if (movementDir == GameConstants::Left)
		next_dir = GameConstants::Right;
	else
		next_dir = GameConstants::Left;

	movementDir = GameConstants::None;//stop moving
}

void Enemy::getEaten()
{
	if (!eaten)
	{
		eaten = true;
		velocityX = 0;
		velocityY = -2;
	}
}

void Enemy::setDrawFrame(Uint32 elapsedTime)
{
	//animation
	//set the state of an object to change the frame
	if(eaten)
		curState = PlayerConstants::Spirit;
	else if (velocityY < 0)
		curState = PlayerConstants::Jumping;
	else if (velocityY > 0)
		curState = PlayerConstants::Falling;
	else if (velocityX != 0 && !inJump)
		curState = PlayerConstants::Running;
	else if (velocityY > -0.09 && velocityY < 0.09 && inJump)//should only be activated while jumping
	{
		curState = PlayerConstants::SuspendedInAir;
	}
	else if (velocityY < 0)
		curState = PlayerConstants::Jumping;
	else if (velocityY > 0)
		curState = PlayerConstants::Falling;
	else
		curState = PlayerConstants::Standing;

	//if player transitioned into another state, reset the frame counter
	if (prevState != curState)
		frameCounter = 0;


	//depending on the state and time passed change frames
	if (curState == PlayerConstants::Spirit)
	{
		elapsedFrameTime += elapsedTime;
		if (elapsedFrameTime >= totalFrameTime)
		{
			frameCounter++;
			if (frameCounter > 2)
				frameCounter = 0;
			elapsedFrameTime = 0;
		}
	}
	else if (curState == PlayerConstants::Running)
	{
		elapsedFrameTime += elapsedTime;
		if (elapsedFrameTime >= totalFrameTime)
		{
			frameCounter++;
			if (frameCounter > 4)
				frameCounter = 0;
			elapsedFrameTime = 0;
		}
	}
	else if (curState == PlayerConstants::Standing)
	{
		elapsedFrameTime += elapsedTime;
		if (elapsedFrameTime >= PlayerConstants::STANDING_SPRITE_CHANGE_TIME)
		{
			frameCounter++;
			if (frameCounter > 2)
				frameCounter = 0;
			elapsedFrameTime = 0;
		}
	}
	//the following three states are all on the third line of spritesheet
	else if (curState == PlayerConstants::Jumping)
	{
		frameCounter = 0;
	}
	else if (curState == PlayerConstants::SuspendedInAir)
	{
		curState = PlayerConstants::Jumping;//change the line of SS used back to 3rd line
		frameCounter = 1;
	}
	else if (curState == PlayerConstants::Falling)
	{
		curState = PlayerConstants::Jumping;//change the line of SS used back to 3rd line
		frameCounter = 2;
	}
	else
		frameCounter = 0;


	//depending on player state set the current frame
	curFrame->y = (SystemConstants::TEXTURE_SIZE + 2) * curState;
	curFrame->x = (SystemConstants::TEXTURE_SIZE + 2) * frameCounter;
	prevState = curState;
}

Enemy::Enemy(SDL_Texture* texture, float x, float y, bool isSolid, bool isDrawable, EnemyConstants::EnemyType type, std::vector<GameObject*>* collisionObjs, int enemyID):GameObject(texture, x, y, true, true, GameConstants::Enemy)
{
	this->id = enemyID;
	drawRect->w /= (int)1.1;
	drawRect->h /= (int)1.1;
	collisionRect->w /= (int)1.1;
	collisionRect->h /= (int)1.1;
	movementDir = GameConstants::Right;//start by moving right
	this->collisionObjs = collisionObjs;
	this->e_type = type;
	//animation support
	curState = PlayerConstants::Standing;
	elapsedFrameTime = 0;
	frameCounter = 0;
	totalFrameTime = GameConstants::SPRITE_CHANGE_TIME;
	prevState = PlayerConstants::Standing;
	inJump = false;
	eaten = false;
	lookAround = false;
	elapsedLookingTime = 0;

	elapsedDeathTime = 0;
	totalDeathTime = EnemyConstants::TOTAL_ENEMY_DEATH_TIME;

	switch (type)
	{
	case EnemyConstants::Bone:
		obj_speed = EnemyConstants::BONE_SPEED;
		break;
	case EnemyConstants::ChickenLeg:
		obj_speed = EnemyConstants::LEG_SPEED;
		break;
	case EnemyConstants::Cheese:
		obj_speed = EnemyConstants::CHEESE_SPEED;
		break;
	}

	if (e_type == EnemyConstants::Hoover)
	{
		/*collisionRect->y -= 16;
		collisionRect->w = EnemyConstants::HOOVER_WIDTH;
		collisionRect->h = EnemyConstants::HOOVER_HEIGHT;*/

		drawRect->y -= 16;
		drawRect->w = EnemyConstants::HOOVER_WIDTH;
		drawRect->h = EnemyConstants::HOOVER_HEIGHT;
	}
}


Enemy::~Enemy()
{
}
