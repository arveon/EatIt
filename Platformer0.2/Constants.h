#ifndef CONST_H
#define CONST_H
#include <string>

//contains game constants such as possible movement directions, velocities, gravity etc
namespace GameConstants
{
	const enum Direction{None = 0, Up = 1, Right = 2, Down = 3, Left = 4};
	const float BASE_OBJECT_VELOCITY = 0.12f;
	const float BASE_JUMP_PENALTY_MULTIPLIER = 1;
	const double GRAVITY = 0.00047;
	const int SPRITE_CHANGE_TIME = 200;

	//size of the camera
	const int CAMERA_WIDTH = 640;
	const int CAMERA_HEIGHT = 480;

	//level constants
	static int LEVEL_WIDTH = 2048;
	static int LEVEL_HEIGHT = 800;

	//object constants
	enum ObjectType {
		Player = 0,
		Enemy = 1,
		Map_Grass = 10,
		Map_Dirt = 11,
		Map_Stone = 12,
		Map_Brick = 13,
		Decoration = 14,
		Map_Spikes = 20,
		Portal_None = 30,
		Portal_A = 31,
		Portal_B = 32,
		Portal_C = 33,
		Portal_D = 34,
		Portal_E = 35,
		Portal_F = 36
	};
}

//will contain system constants such as window dimensions
namespace SystemConstants
{

	static const int WINDOW_WIDTH = 1015;
	static const int WINDOW_HEIGHT = 700;

	static const float SCALING_H = ((float)WINDOW_WIDTH / GameConstants::CAMERA_WIDTH);
	static const float SCALING_V = ((float)WINDOW_HEIGHT / GameConstants::CAMERA_HEIGHT);

	/* //my screen resolution
	static const int WINDOW_WIDTH = 1366;
	static const int WINDOW_HEIGHT = 768;
	*/

	//cap framerate vars
	static const int FRAMERATE_CAP = 60;
	static const float TIME_PER_FRAME = (float)1000 / FRAMERATE_CAP;
	static const int FRAMERATE_UPDATE_TIME = 50;

	static const std::string LEVEL_0_0 = "maps/1/level_a.txt";
	static const std::string LEVEL_0 = "level_a.txt";
	static const std::string LEVEL_1 = "level_b.txt";
	static const std::string LEVEL_2 = "level_c.txt";
	static const std::string LEVEL_3 = "level_d.txt";
	static const std::string MAPS_FOLDER = "maps/";

	static const int TEXTURE_SIZE = 32;

	const enum GameState
	{
		Menu = 0,
		GameGoing = 1,
		Exit = 2
	};
}

namespace MenuConstants
{
	const enum MenuType
	{
		MainMenu = 0,
		PauseMenu = 1,
		GameOverMenu = 2
	};

	const enum ButtonType
	{
		None = 0,
		Exit = 1,
		ExitToMenu = 2,
		StartGame = 3,
		ContinueGame = 4
	};

	static const int BUTTON_TEXTURE_WIDTH = 100;
	static const int BUTTON_TEXTURE_HEIGHT = 16;

	static const int PAUSE_MENU_WINDOW_WIDTH = 250;
	static const int PAUSE_MENU_WINDOW_HEIGHT = 300;

	static const int PAUSE_MENU_RESUME_X_OFF = 20;
	static const int PAUSE_MENU_RESUME_Y_OFF = 100;
	static const int PAUSE_MENU_EXIT_X_OFF = 30;
	static const int PAUSE_MENU_EXIT_Y_OFF = 200;

	static const int MAIN_MENU_START_X = 400;
	static const int MAIN_MENU_START_Y = 100;
	static const int MAIN_MENU_EXIT_X = 400;
	static const int MAIN_MENU_EXIT_Y = 200;

	static const int MAIN_MENU_BUTTON_WIDTH = 200;
	static const int MAIN_MENU_BUTTON_HEIGHT = 32;

	static const int PAUSE_MENU_CONTINUE_X = 50;
	static const int PAUSE_MENU_CONTINUE_Y = 50;
	static const int PAUSE_MENU_EXIT_X = 50;
	static const int PAUSE_MENU_EXIT_Y = 145;
}

//map constants
namespace MapConstants
{
	const int TILE_WIDTH = 32;
	const int TILE_HEIGHT = 32;

	const int SPIKES_HEIGHT = 16;

	const int TILES_X = GameConstants::LEVEL_WIDTH / TILE_WIDTH;
	const int TILES_Y = GameConstants::LEVEL_HEIGHT / TILE_HEIGHT;
}

namespace PlayerConstants
{
	const int PLAYER_WIDTH = 32;
	const int PLAYER_HEIGHT = 32;
	const float PLAYER_SPEED = 0.19f;
	const float JUMP_SPEED = 0.31f;
	const float JUMP_X_PENALTY_MULT = 1.5f;

	const enum PLAYER_STATE {
		Standing = 0,
		Running = 1,
		Jumping = 2,
		Spirit = 3,
		SuspendedInAir = 4,
		Falling = 5
	};
	const int SPRITE_CHANGE_TIME = 200;
	const int STANDING_SPRITE_CHANGE_TIME = 800;

	const int START_LIVES = 5;

	const int FALLING_DAMAGE_HEIGHT = 6;

	const int DAMAGE_FLASHING_FREQUENCY = 300;
	const int NUM_FLASHES = 3;

	const float CHICKEN_LEG_JUMP_BONUS = 0.0225f;
	const float BONE_PENALTY_MULT_BONUS = 0.35f;
}

namespace EnemyConstants
{
	const int HOOVER_WIDTH = 48;
	const int HOOVER_HEIGHT = 48;

	const float BONE_SPEED = 0.07f;
	const float LEG_SPEED = 0.1f;
	const float CHEESE_SPEED = 0.07f;

	enum EnemyType
	{
		ChickenLeg = 1,
		Cheese = 2,
		Bone = 3,
		Hoover = 4
	};
	
	static int TOTAL_ENEMY_DEATH_TIME = 1000;
	static int TOTAL_STANDING_TIME = 2000;
}

namespace SoundConstants
{
	const std::string BG_MUSIC = "sounds/bg_music.wav";
	const std::string WIN = "sounds/win_sound.wav";
	const std::string SECONDARY_MENU = "sounds/page_flip.wav";
	const std::string BUTTON_CLICK = "sounds/button_click.wav";
	const std::string PLAYER_RUN = "sounds/dog_run_e.wav";
	const std::string PLAYER_JUMP = "sounds/game_pick_up_object.wav";
	const std::string PLAYER_EAT = "sounds/cartoon_bite.wav";
	const std::string PLAYER_STAND = "sounds/dog_pant.wav";
	const std::string PLAYER_CRY = "sounds/dog_talking.wav";

	const int BUTTONS_CHANNEL = 1;
	const int PLAYER_CHANNEL = 2;
	const int ENEMIES_CHANNEL = 3;
}
#endif