#include "Camera.h"

void Camera::Update(int centerX, int centerY)
{
	//update the coordinates of the camera
	x = centerX - cameraRect->w/2;
	y = centerY - cameraRect->h/2;
	
	//contain camera within level bounds
	if (x < 0)
		x = 0;
	else if (x + cameraRect->w > GameConstants::LEVEL_WIDTH)
		x = GameConstants::LEVEL_WIDTH - cameraRect->w;
	if (y < 0)
		y = 0;
	else if (y + cameraRect->h > GameConstants::LEVEL_HEIGHT)
		y = GameConstants::LEVEL_HEIGHT - cameraRect->h;

	//update the camera rectangle
	cameraRect->x = x;
	cameraRect->y = y;
}

//initialise camera to initial coordinates
Camera::Camera(int x, int y)
{
	//set coordinates
	this->x = x;
	this->y = y;

	//create a rectangle
	cameraRect = new SDL_Rect{x, y, GameConstants::CAMERA_WIDTH, GameConstants::CAMERA_HEIGHT};
}

//deallocate the camera
Camera::~Camera()
{
	delete cameraRect;
}
