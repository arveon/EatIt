#include "SoundManager.h"



bool SoundManager::loadSounds()
{
	bool success = true;
	button_click_snd = Mix_LoadWAV(SoundConstants::BUTTON_CLICK.c_str());
	assert(button_click_snd);
	button_click_snd->volume = 50;

	player_run_snd = Mix_LoadWAV(SoundConstants::PLAYER_RUN.c_str());
	assert(player_run_snd);
	player_run_snd->volume = 40;

	player_eats_snd = Mix_LoadWAV(SoundConstants::PLAYER_EAT.c_str());
	assert(player_eats_snd);
	player_eats_snd->volume = 30;

	player_stand_snd = Mix_LoadWAV(SoundConstants::PLAYER_STAND.c_str());
	assert(player_stand_snd);
	player_stand_snd->volume = 20;

	player_jump_snd = Mix_LoadWAV(SoundConstants::PLAYER_JUMP.c_str());
	assert(player_jump_snd);
	player_jump_snd->volume = 10;

	player_cry_snd = Mix_LoadWAV(SoundConstants::PLAYER_CRY.c_str());
	assert(player_cry_snd);
	player_cry_snd->volume = 30;

	win_snd = Mix_LoadWAV(SoundConstants::WIN.c_str());
	assert(win_snd);
	win_snd->volume = 20;

	secondary_menu = Mix_LoadWAV(SoundConstants::SECONDARY_MENU.c_str());
	assert(secondary_menu);
	secondary_menu->volume = 20;

	//music
	bg_music = Mix_LoadMUS(SoundConstants::BG_MUSIC.c_str());
	assert(bg_music);
	Mix_VolumeMusic(10);

	return success;
}


SoundManager::SoundManager()
{
}


SoundManager::~SoundManager()
{
	Mix_FreeChunk(button_click_snd);
}


void SoundManager::buttonClick()
{
	if(!Mix_Playing(SoundConstants::BUTTONS_CHANNEL))
		Mix_PlayChannel(SoundConstants::BUTTONS_CHANNEL, button_click_snd, 0);
}

void SoundManager::playerSound(PlayerConstants::PLAYER_STATE state, PlayerConstants::PLAYER_STATE prev_state)
{
	if (!Mix_Playing(SoundConstants::PLAYER_CHANNEL))
	{
		if(state == PlayerConstants::Running)
			Mix_PlayChannel(SoundConstants::PLAYER_CHANNEL, player_run_snd, 0);
		
		if(state == PlayerConstants::Standing)
			Mix_PlayChannel(SoundConstants::PLAYER_CHANNEL, player_stand_snd, 0);
	}

	if (state == PlayerConstants::Jumping)
		if ((prev_state != PlayerConstants::Jumping && prev_state != PlayerConstants::SuspendedInAir && prev_state != PlayerConstants::Falling))
			Mix_PlayChannel(SoundConstants::PLAYER_CHANNEL, player_jump_snd, 0);
}

void SoundManager::damageTaken()
{
	Mix_PlayChannel(SoundConstants::PLAYER_CHANNEL, player_cry_snd, 0);
}

void SoundManager::playEat()
{
	Mix_PlayChannel(SoundConstants::ENEMIES_CHANNEL, player_eats_snd, 0);
}

void SoundManager::startMusic()
{
	if (Mix_PlayingMusic() == 0)
		Mix_PlayMusic(bg_music, -1);
}

void SoundManager::stopMusic()
{
	Mix_HaltMusic();
}

void SoundManager::playWin()
{
	Mix_PlayChannel(SoundConstants::BUTTONS_CHANNEL, win_snd, 0);
}

void SoundManager::playSecondaryMenu()
{
	Mix_PlayChannel(SoundConstants::BUTTONS_CHANNEL, secondary_menu, 0);
}
