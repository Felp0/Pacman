#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

//Defining
#define MUNCHIECOUNT 188
#define BLOCKCOUNT 185
#define GHOSTCOUNT 5

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.

struct Player 
{
	const float cSpeed = 0.6f;
	const int cFrameTime = 250;

	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
	bool dead;
	float speedMultuplier;
	int currentFrameTime;
	int direction;
	int frame;
	int scoreCount;
};

struct Enemy
{
	const int cMunchieFrameTime = 500;

	int frameCount;
	int frame;
	int currentFrameTime;
	Rect* rect;
	Texture2D* texture;
	Texture2D* invertedTexture;
	Vector2* position;

};

struct Menu 
{
	Texture2D* background;
	Texture2D* startGame;
	Rect* rectangle;
	Rect* startRectangle;
	Vector2* stringPosition;
	Vector2* startString1;
	Vector2 startString2;

};

struct Map
{
	Rect* rectangle;
	Texture2D* texture;
	Vector2* position;

};

struct MovingEnemy
{
	const int cFrametime = 250;

	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	int currentFrameTime;
	int frame;
	float speed;
};

class Pacman : public Game
{
private:

	int aMap[768] =
	{	1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,2,2,2,2,2,0,0,0,0,0,0,0,0,1,2,2,2,0,0,0,0,0,0,0,0,0,0,2,2,1,
		1,1,2,2,2,2,2,0,0,0,0,0,0,0,0,1,2,2,2,0,0,2,2,2,0,0,0,0,0,2,2,1,
		1,1,2,2,2,2,2,0,0,0,0,0,0,0,0,1,2,2,2,0,0,2,1,2,0,0,0,0,0,2,2,1,
		1,1,2,2,2,2,2,0,0,0,0,3,0,0,0,1,2,2,2,0,0,2,1,2,0,0,0,0,0,2,2,1,
		1,1,2,2,2,2,2,0,0,0,0,0,0,0,0,1,2,2,2,0,0,2,1,2,0,0,3,0,0,2,2,1,
		1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,2,1,2,0,0,0,0,0,2,2,1,
		1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,2,2,1,0,0,1,1,1,1,1,0,0,0,2,2,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,1,1,1,1,2,2,2,1,1,0,0,0,1,1,
		0,0,0,0,0,0,0,0,0,3,0,0,0,2,0,1,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,0,
		0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,
		0,2,2,2,2,2,0,1,1,1,0,0,0,1,1,1,2,0,0,0,0,0,0,0,0,0,3,0,0,0,2,0,
		0,2,2,2,2,2,0,1,2,2,2,0,0,0,0,1,2,0,0,0,0,0,0,0,0,1,0,0,0,0,2,0,
		1,2,2,2,2,2,0,1,2,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,1,0,0,0,0,2,1,
		1,1,1,0,0,1,1,1,2,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,1,2,2,2,0,2,1,
		1,0,0,0,0,0,0,0,2,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,2,0,0,1,
		1,0,0,0,3,0,2,2,2,0,0,0,0,1,2,2,2,0,0,0,0,0,0,2,1,1,2,2,2,0,0,1,
		1,0,0,0,0,0,2,0,0,0,0,0,0,1,2,2,2,0,0,0,0,0,2,2,1,1,2,0,0,0,0,1,
		0,0,0,0,0,0,2,1,0,0,0,0,1,1,2,2,2,0,0,0,0,0,2,2,1,1,2,0,0,0,0,0,
		0,2,2,2,2,2,2,1,0,0,0,0,1,2,2,2,0,0,0,0,0,0,2,2,1,1,2,0,0,0,0,0,
		1,2,2,2,2,2,2,1,0,0,0,0,1,2,2,0,0,0,0,0,0,0,2,2,1,1,2,2,2,2,2,1,
		1,2,2,2,2,2,2,1,0,0,0,0,1,2,2,0,0,0,0,0,0,0,2,2,1,1,2,2,2,2,2,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		
	};

	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewPortCollision();
	//Enemies
	void CheckGhostCollisions(MovingEnemy*, int elapsedTime);
	void UpdateGhost(int elapsedTime);

	//Update methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(Enemy* munchie , int elapsedTime);

	//Pointers (struct)
	Player* _pacman;
	Menu* _menu;
	Enemy* _munchies[MUNCHIECOUNT];
	Map* _map[BLOCKCOUNT];
	MovingEnemy* _ghosts[GHOSTCOUNT];

	//Bool for menu
	bool _gameStarted;
	bool _spaceKeyDown;
	bool _paused;
	bool _pkeyDown;
	
	// Position for String
	Vector2* _stringPosition;

	


public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	//Collision
	bool CollisionCheck(int x1, int y1, int widht1, int height1,
		int x2, int y2, int width2, int height);

	//Mouse Input
	void Input(int elapsedTime, Input::KeyboardState* kState, Input::MouseState* moState);
};