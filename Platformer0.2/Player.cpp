#include "Player.h"

//player update logic
void Player::Update(Uint32 elapsedTime, const Uint8* keyboardState, GameObject*** map)
{
	//std::cout << "0" << std::endl;
	//while falling, calculate if the falling damage should be applied upon falling
	if (velocityY > 0)
	{
		fallingDistance += velocityY * elapsedTime;//calculate the fall distance
		if (fallingDistance / MapConstants::TILE_HEIGHT > PlayerConstants::FALLING_DAMAGE_HEIGHT)//if fell down more than num tiles set in constants, set fall damage flag
		{
			fallingDamageFlag = true;
			fallingDistance = 0;
		}
	}
	//std::cout << "1" << std::endl;
	if (onGround && fallingDamageFlag)//need to be separate because only applies when player on the ground
	{
		takeDamage(false);
		fallingDamageFlag = false;
	}
	else if (onGround)
	{
		fallingDistance = 0;
		inJump = false;
	}

	//std::cout << "2" << std::endl;
	//change movement direction if one of the movement keys is pressed
	if (keyboardState[SDL_SCANCODE_W])
	{
		if (onGround && !jumpPressed)
		{
			velocityY = -jumpImpulse;
			y -= 1;
			onGround = false;
			jumpPressed = true;//required to prevent holding the key and jumping continuously
			inJump = true;
		}
	}
	else
		jumpPressed = false;//reset jumpPressed if W released

	//std::cout << "3" << std::endl;
	//depending on one of the keys pressed, set movement direction
	if (keyboardState[SDL_SCANCODE_A])
		movementDir = GameConstants::Direction::Left;
	else if (keyboardState[SDL_SCANCODE_D])
		movementDir = GameConstants::Direction::Right;
	else
		movementDir = GameConstants::None;

	//std::cout << "4" << std::endl;
	//let player toggle nogravity mode (can fly)
	if (keyboardState[SDL_SCANCODE_G] && !noGravityPressed)
	{
		noGravityMode = !noGravityMode;
		noGravityPressed = true;
	}
	else if (!keyboardState[SDL_SCANCODE_G])
		noGravityPressed = false;

	//std::cout << "5" << std::endl;
	//allows player enter 'cheat mode'
	if (noGravityMode)
	{
		if (keyboardState[SDL_SCANCODE_W])
			velocityY = -PlayerConstants::PLAYER_SPEED;
		else if (keyboardState[SDL_SCANCODE_S])
			velocityY = PlayerConstants::PLAYER_SPEED;
		else
			velocityY = 0;

		fallingDistance = 0;
		fallingDamageFlag = false;
	}

	//std::cout << "6" << std::endl;
	//apply x speed penalty if in jump
	if (inJump && !noGravityMode)
		obj_speed = PlayerConstants::PLAYER_SPEED / jumpXPenaltyMultiplier;
	else
		obj_speed = PlayerConstants::PLAYER_SPEED;

	//std::cout << "7" << std::endl;
	//actually move player
	GameObject::Update(elapsedTime, map);

	////reset injump if landed on the ground
	//if (onGround && inJump)
	//{
	//	inJump = false;
	//}

	//std::cout << "8" << std::endl;
	//check for player collision with stuff
	bool brout = false;
	for (int i = collisionObjs->size() - 1; i >= 0; i--)
	{
		if (SDL_HasIntersection(collisionObjs->at(i)->getCollRectangle(), collisionRect))
		{
			switch (collisionObjs->at(i)->getType())
			{
			case GameConstants::Map_Spikes:
				fallingDistance = 0;
				fallingDamageFlag = false;
				takeDamage(true);
				brout = true;
				break;
			case GameConstants::Portal_A:
				inPortal = GameConstants::Portal_A;
				break;
			case GameConstants::Portal_B:
				inPortal = GameConstants::Portal_B;
				break;
			case GameConstants::Portal_C:
				inPortal = GameConstants::Portal_C;
				break;
			case GameConstants::Portal_D:
				inPortal = GameConstants::Portal_D;
				break;
			case GameConstants::Portal_E:
				inPortal = GameConstants::Portal_E;
				break;
			case GameConstants::Portal_F:
				inPortal = GameConstants::Portal_F;
				break;
			}
		}
		if (brout)
			break;
	}
	//std::cout << "9" << std::endl;
	animate(elapsedTime);
}

//draw the player (a weird link error appears if it's removed)
void Player::Draw(SDL_Renderer* renderer, SDL_Rect* cameraRect)
{
	GameObject::Draw(renderer, cameraRect);
}

//initialise the player
Player::Player(SDL_Texture* texture, float x, float y):GameObject(texture, x, y, true, true, GameConstants::Player)
{
	//set the rectangle sizes after the GameObject constructor is called
	drawRect->w = PlayerConstants::PLAYER_WIDTH;
	drawRect->h = PlayerConstants::PLAYER_HEIGHT;
	setCoordinates((int)x, (int)y);
	collisionRect->w = PlayerConstants::PLAYER_WIDTH;
	collisionRect->h = PlayerConstants::PLAYER_HEIGHT;
	jumpPressed = false;
	obj_speed = PlayerConstants::PLAYER_SPEED;
	//animation support
	curState = PlayerConstants::Standing;
	elapsedFrameTime = 0;
	frameCounter = 0;
	totalFrameTime = PlayerConstants::SPRITE_CHANGE_TIME;
	prevState = PlayerConstants::Standing;
	inJump = false;
	jumpImpulse = PlayerConstants::JUMP_SPEED;
	jumpXPenaltyMultiplier = PlayerConstants::JUMP_X_PENALTY_MULT;
	//noGravityMode = false;
	lives = PlayerConstants::START_LIVES;
	damageAnimation = false;
	fallingDamageFlag = false;
	fallingDistance = 0;
	killedEnemy = 0;
	finish = false;
}

void Player::resetBetweenLevels()
{
	currentRoom = GameConstants::Portal_A;
	inPortal = GameConstants::Portal_None;
}

Player::~Player()
{
}

void Player::setCoordinates(int x, int y)
{
	this->x = (float)x;
	this->y = (float)y;
	collisionRect->x = x;
	collisionRect->y = y;
	drawRect->x = x;
	drawRect->y = y;
	curState = PlayerConstants::Standing;
	elapsedFrameTime = 0;
	fallingDamageFlag = false;
	fallingDistance = 0;
}

bool Player::canEat(EnemyConstants::EnemyType enemyType)
{
	//replace with something more specific. Player needs to be attacking to be able to eat
	//add 3 combat states: attacking, passive, taking damage(?)
	//only able to eat while attacking
	//maybe

	//hoover can be killed only by falling on it
	if (enemyType == EnemyConstants::Hoover)
		if (velocityY > 0)
			return true;
		else
			return false;
	else if(enemyType == EnemyConstants::Hoover)
		return false;

	return true;
}

void Player::feed(EnemyConstants::EnemyType enemyType, int id)
{
	switch (enemyType)
	{
	case EnemyConstants::ChickenLeg:
		jumpImpulse += PlayerConstants::CHICKEN_LEG_JUMP_BONUS;
		break;
	case EnemyConstants::Bone:
		jumpXPenaltyMultiplier -= PlayerConstants::BONE_PENALTY_MULT_BONUS;
		break;
	case EnemyConstants::Cheese:
		lives++;
		std::cout << "Life gained: " << lives << std::endl;
		break;
	}
	killedEnemy = id;
}

void Player::animate(int elapsedTime)
{
	prevState = curState;
	//animation
	//set the state of an object to change the frame
	if (velocityX != 0 && !inJump)
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
	if (curState == PlayerConstants::Running)
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
			if (frameCounter > 1)
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
		//curState = PlayerConstants::Jumping;//change the line of SS used back to 3rd line
		frameCounter = 1;
	}
	else if (curState == PlayerConstants::Falling)
	{
		//curState = PlayerConstants::Jumping;//change the line of SS used back to 3rd line
		frameCounter = 2;
	}
	else
		frameCounter = 0;


	//depending on player state set the current frame
	if (curState != PlayerConstants::Jumping && curState != PlayerConstants::Falling && curState != PlayerConstants::SuspendedInAir)
		curFrame->y = SystemConstants::TEXTURE_SIZE * curState;
	else
		curFrame->y = SystemConstants::TEXTURE_SIZE * 2;

	curFrame->x = (SystemConstants::TEXTURE_SIZE + 2) * frameCounter;
	//prevState = curState;

	//std::cout << "10" << std::endl;
	//add flashing upon damage
	static int numFlashes;//needs to be static to keep value between calls
	if (damageAnimation)
	{
		elapsedDamageAnimationTime += elapsedTime;
		if (elapsedDamageAnimationTime >= PlayerConstants::DAMAGE_FLASHING_FREQUENCY)//change state if time has come
		{
			isDrawable = !isDrawable;
			elapsedDamageAnimationTime = 0;
			numFlashes++;
		}

		//if limit of flashes reached, stop animation
		if (numFlashes / 2 >= PlayerConstants::NUM_FLASHES)//need /2 to count only full flashes (off+on as one)
			damageAnimation = false;
	}
	else
		numFlashes = 0;//resets the elapsed number of flashes
}

void Player::takeDamage(bool needsRestart)
{
	std::cout << "taken damage! lives remaining: " << (lives - 1) << std::endl;
	if (needsRestart)
	{
		fallingDamageFlag = false;
		fallingDistance = 0;
		inPortal = currentRoom;
	}
	else
		damageAnimation = true;
	lives--;

	tookDamage = true;

	if (lives <= 0)
		isActive = false;
}