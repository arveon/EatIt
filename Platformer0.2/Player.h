#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

#include "GameObject.h"
#include "Constants.h"
#include "TileMap.h"

#include <iostream>

class Player : public GameObject
{
private:
	bool finish;

	bool jumpPressed;//needed to prevent jumping while holding the key
	bool inJump;
	float jumpImpulse;
	float jumpXPenaltyMultiplier;
	int lives;

	//static bool noGravityMode;
	bool noGravityPressed;

	//animation support
	PlayerConstants::PLAYER_STATE curState;
	PlayerConstants::PLAYER_STATE prevState;
	int frameCounter;
	int elapsedFrameTime;
	int totalFrameTime;

	

	bool fallingDamageFlag;
	float fallingDistance;
	bool damageAnimation;
	int elapsedDamageAnimationTime;
	int flashes;

	GameConstants::ObjectType inPortal;
	GameConstants::ObjectType currentRoom;
	void animate(int elapsedTime);
public:
	void Update(Uint32 elapsedTime, const Uint8* keyboardState, GameObject*** map);
	void Draw(SDL_Renderer* renderer, SDL_Rect* cameraRect);

	Player(SDL_Texture* texture, float x, float y);
	~Player();

	void setCoordinates(int x, int y);
	void setActive() { isActive = true; }
	//methods required to properly move between rooms
	void resetPortal() { inPortal = GameConstants::Portal_None; }

	void setCurrentRoom(GameConstants::ObjectType newCurRoom) { currentRoom = newCurRoom; }
	void resetBetweenLevels();
	bool canEat(EnemyConstants::EnemyType enemyType);
	void feed(EnemyConstants::EnemyType enemyType, int enemyID);
	void takeDamage(bool needsRestart);

	
	void finished() { finish = true; }
	bool isFinished() { return finish; }

	int killedEnemy;
	bool tookDamage;

	GameConstants::ObjectType getPortal() { return inPortal; }
	GameConstants::ObjectType getCurrentRoom() { return currentRoom; }
	PlayerConstants::PLAYER_STATE getState() { return curState; }
	PlayerConstants::PLAYER_STATE getPrevState() { return prevState; }
	int getLives() { return lives; }

	void setLives(int lc) { lives = lc; }

	//float getImpulse() { return jumpImpulse; }
	//float getLengthMultiplier() { return jumpXPenaltyMultiplier; }
};

#endif