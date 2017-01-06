#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include <SDL.h>
#include <vector>
#include <iostream>

#include "Constants.h"

class GameObject
{
protected:
	//object texture
	SDL_Texture* texture = nullptr;
	SDL_Rect* curFrame;

	//object positioning
	SDL_Rect* collisionRect = nullptr;//position on the map
	SDL_Rect* drawRect = nullptr;//position on the screen
	float x, y;

	//movement support
	float velocityX, velocityY;
	GameConstants::Direction movementDir;
	GameConstants::Direction facingDir;
	float obj_speed;
	//bool jumping;
	bool onGround;
	
	

	//object properties
	GameConstants::ObjectType type;
	bool isSolid;
	bool isDrawable;
	bool isActive;
	//bool mapObject;

	//objects that this game object might collide with
	std::vector<GameObject*>* collisionObjs;

	bool noGravityMode;
public:
	GameObject(SDL_Texture* texture, float x, float y, bool isSolid, bool isDrawable, GameConstants::ObjectType type);
	~GameObject();
	virtual void Update(Uint32 elapsedTime, GameObject*** map);
	virtual void Draw(SDL_Renderer* renderer, SDL_Rect* cameraRect);
	bool isObjectDrawable() { return isDrawable; }
	float getOriginX() { return x; }
	float getOriginY() { return y; }
	SDL_Rect* getCollRectangle() { return collisionRect; }
	void resolveCollisionsX(GameObject*** map, int axis);
	bool isObjectActive() { return isActive; }
	void setCollisionObjects(std::vector<GameObject*>* collisionObjects) { this->collisionObjs = collisionObjects; }
	GameConstants::ObjectType getType() { return type; }
	bool isObjSolid() { return isSolid; }
};

#endif