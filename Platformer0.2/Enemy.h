#ifndef ENEMY_H
#define ENEMY_H
#include "iostream"
#include "string"

#include "GameObject.h"

//a base enemy class
class Enemy : public GameObject
{
private:
	//different enemies support
	EnemyConstants::EnemyType e_type;

	//animation support
	PlayerConstants::PLAYER_STATE curState;
	PlayerConstants::PLAYER_STATE prevState;
	int frameCounter;
	int elapsedFrameTime;
	int totalFrameTime;
	bool inJump;
	bool eaten;
	int elapsedDeathTime;
	int totalDeathTime;

	bool lookAround;
	GameConstants::Direction next_dir;
	int elapsedLookingTime;

	int id;

	void jump();//makes the enemy jump
	void changeDirection();//changes the movement direction to opposite
	void setDrawFrame(Uint32 elapsedTime);
public:
	Enemy(SDL_Texture* texture, float x, float y, bool isSolid, bool isDrawable, EnemyConstants::EnemyType type, std::vector<GameObject*>* collisionObjs, int enemyID);
	~Enemy();

	void Update(Uint32 elapsedTime, GameObject*** map);
	void Draw(SDL_Renderer* renderer, SDL_Rect* cameraRect);
	void getEaten();
	bool isEaten() { return eaten; }
	EnemyConstants::EnemyType getEnemyType() { return e_type; }
	int getID() { return id; }
};

#endif