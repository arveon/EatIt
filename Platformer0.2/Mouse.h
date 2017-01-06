#include <SDL.h>

class Mouse
{
private:
	SDL_Rect draw_rect;
	SDL_Rect cur_frame;
	SDL_Texture* mouse_t;

public:
	Mouse(SDL_Texture* mouse_t);
	~Mouse();
	void Update(int mouse_x, int mouse_y, bool LMB_Down);
	void Draw(SDL_Renderer* renderer);
};

