#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
#include "Constants.h"

class Camera
{
private:
	//the position of the camera
	int x;
	int y;
	//camera rectangle
	SDL_Rect* cameraRect;

public:
	void Update(int centerX, int centerY);
	SDL_Rect* getCameraRect(){	return cameraRect;	}
	Camera(int x, int y);
	~Camera();
};
#endif