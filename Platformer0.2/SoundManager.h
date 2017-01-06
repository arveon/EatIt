#include <SDL_mixer.h>

#include <iostream>
#include <cassert>

#include "Constants.h"

class SoundManager
{
private:
	Mix_Music* bg_music = nullptr;

	Mix_Chunk* button_click_snd = nullptr;

	Mix_Chunk* player_run_snd = nullptr;
	Mix_Chunk* player_jump_snd = nullptr;
	Mix_Chunk* player_eats_snd = nullptr;
	Mix_Chunk* player_stand_snd = nullptr;
	Mix_Chunk* player_cry_snd = nullptr;
	Mix_Chunk* win_snd = nullptr;
	Mix_Chunk* secondary_menu = nullptr;

	Mix_Chunk* enemy_eaten_snd = nullptr;
public:
	bool loadSounds();
	SoundManager();
	~SoundManager();

	void startMusic();
	void stopMusic();

	void buttonClick();
	void playEat();
	void playSecondaryMenu();
	void playerSound(PlayerConstants::PLAYER_STATE, PlayerConstants::PLAYER_STATE);
	void damageTaken();
	void playWin();
};

