#include "Pacman.h"

#include <sstream>
#include <time.h>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	srand(time(NULL));

	//Initialise member variables
	_pacman = new Player();
	_menu = new Menu();
	
	//Munchies
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Enemy();
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frameCount = 0;
		_munchies[i]->frame = rand() % 500 * 50;

	
	}
	//Special Munchies
	for (int i = 0; i < SPECIALMUNCHIE; i++)
	{
		_specialMunchie[i] = new SpecialMunchie();
		_specialMunchie[i]->currentFrameTime = 0;
		_specialMunchie[i]->frameCount = 0;
		_specialMunchie[i]->frame = rand() % 500 * 50;
	}
	//Map
	for (int i = 0; i < BLOCKCOUNT; i++) {
		{
			_map[i] = new Map();
		
		}
	}

	//Initialise ghost character
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new MovingEnemy();
		_ghosts[i]->direction = 0;
		_ghosts[i]->speed = 0.2f;
		_ghosts[i]->frame = rand() % 500 * 50;
		_ghosts[i]->currentFrameTime = 0;
		 

	
	}

	_paused = false;
	_pkeyDown = false;
	_gameStarted = true;

	//pacman
	_pacman->direction = 0;
	_pacman->currentFrameTime = 0;
	_pacman->frame = 0;
	_pacman->speedMultuplier = 1.0f;
	_pacman->dead = false;
	_pacman->scoreCount = 0;
	

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 736, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	
	delete _munchies[0]->texture;
	delete _ghosts[0]->texture;
	
	for (int nCount = 0; nCount < MUNCHIECOUNT; nCount++)
	{
		delete _munchies[nCount]->position;
		delete _munchies[nCount]->rect;
		delete _munchies[nCount];
		
	}
	delete[] _munchies;

	for (int nCount = 0; nCount < GHOSTCOUNT; nCount++)
	{
		delete _ghosts[nCount]->position;
		delete _ghosts[nCount]->sourceRect;
		delete _ghosts[nCount];
	}
	delete[] _ghosts;
	
	for (int nCount = 0; nCount < SPECIALMUNCHIE; nCount++)
	{
		delete _specialMunchie[nCount]->position;
		delete _specialMunchie[nCount]->rect;
		delete _specialMunchie[nCount];
	}
	delete[] _specialMunchie;
}

void Pacman::LoadContent()
{
	//Set StartMenu Paramters
	_menu->startGame = new Texture2D();
	_menu->startGame->Load("Textures/Menu.jpg", true);
	_menu->startRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->startString1 = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 3.0f);
	// Set PauseMenu Paramters
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	//Load death screen
	_menu->deathscreen = new Texture2D();
	_menu->deathscreen->Load("Textures/deathScreen.png", true);
	_menu->rect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/sprites.png", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Ghost Character
	Texture2D* ghostTex = new Texture2D();
	ghostTex->Load("Textures/Enemy.png", false);

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->texture = ghostTex;
		_ghosts[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	}
	// Load munchie
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png", false);

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = munchieTex;
		_munchies[i]->texture->Load("Textures/Munchie.png", false);
		_munchies[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_munchies[i]->rect = new Rect(0.0f, 0.0f, 12, 12);
	}
	//Load SpecialMunchie
	Texture2D* specialTex = new Texture2D();
	specialTex->Load("Textures/Scroll.png", false);

	for (int i = 0; i < SPECIALMUNCHIE; i++)
	{
		_specialMunchie[i]->texture = specialTex;
		_specialMunchie[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_specialMunchie[i]->rect = new Rect(0.0f, 0.0f, 12, 12);
		
	}

	//Load map
	Texture2D* emptyMap = new Texture2D();
	emptyMap->Load("Textures/Map.jpg", false);	

	//initialize blocks
	for (int i = 0; i < BLOCKCOUNT; i++)
	{

			_map[i]->texture = emptyMap;
			_map[i]->rectangle = new Rect{ 0.0f, 0.0f, 32,32 };
			_map[i]->position = new Vector2(0, 0);
	}

	//Making map
	for (int i = 0,munchies = 0,map = 0, ghost = 0, special = 0; i < 768; i++) 
	{
			
			if (aMap[i] == 1) 
			{
				_map[map]->position = new Vector2((i%32)*32, (i/32)*32);
				map++;
			}
			if (aMap[i] == 2)
			{
				_munchies[munchies]->position = new Vector2((i % 32) * 32, (i / 32) * 32);
				munchies++;

			}
			if (aMap[i] == 3)
			{
				_ghosts[ghost]->position = new Vector2((i % 32) * 32, (i / 32) * 32);
				ghost++;
			}
			if (aMap[i] == 4)
			{
				_specialMunchie[special]->position = new Vector2((i % 32) * 32, (i / 32) * 32);
				special++;
			}
			

	}
	// Set string position
	_stringPosition = new Vector2(21.48f, 15.36f);

	
}


//Updates
void Pacman::Update(int elapsedTime)
{
	
	// Gets the current state of the keyboard and Mouse
	Input::KeyboardState* kState = Input::Keyboard::GetState();	
	Input::MouseState* mState = Input::Mouse::GetState();



	//Start Menu
	if (kState->IsKeyDown(Input::Keys::SPACE))
	{
		_gameStarted = false;
	}
	else
	{
		CheckPaused(kState, Input::Keys::P);

		if (!_paused && _gameStarted == false)
		{
			//Input function
			Input(elapsedTime, kState, mState);

			//Update function
			UpdateGhost(elapsedTime);

			//Check functions
			CheckViewPortCollision();
			CheckGhostCollisions();
			
			


			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				UpdateMunchie(_munchies[i], elapsedTime);
				if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
					_munchies[i]->position->X, _munchies[i]->position->Y, _munchies[i]->rect->Width, _munchies[i]->rect->Height))
				{
					_munchies[i]->position->X = -100;
					_munchies[i]->position->Y = -100;
					_pacman->scoreCount++;
				}
			}

			for (int i = 0; i < SPECIALMUNCHIE; i++)
			{
				UpdateSpecial(_specialMunchie[i], elapsedTime);
				if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
					_specialMunchie[i]->position->X, _specialMunchie[i]->position->Y, _specialMunchie[i]->rect->Width, _specialMunchie[i]->rect->Height))
				{
					_specialMunchie[i]->position->X = -100;
					_specialMunchie[i]->position->Y = -100;
					_pacman->scoreCount += 100;
				}
			}

			for(int i = 0; i < BLOCKCOUNT; i++)
			{
				if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
					_map[i]->position->X, _map[i]->position->Y, _map[i]->rectangle->Width, _map[i]->rectangle->Height))
				{
					//Hero
					if (_pacman->direction == 0)
					{
						_pacman->position->X -= 10;
					}

					if (_pacman->direction == 2)
					{
						_pacman->position->X += 10;
					}

					if (_pacman->direction == 3)
					{
						_pacman->position->Y += 10;
					}

					if (_pacman->direction == 1)
					{
						_pacman->position->Y -= 10;
					}

				
				}
			}


		}
	}
		
}

void Pacman::UpdateMunchie(Enemy* munchie, int elapsedTime)
 {
	
	 //Munchie Frame 
	
		munchie->currentFrameTime += elapsedTime;
		if (munchie->currentFrameTime > munchie->cMunchieFrameTime)
		{
			munchie->frameCount++;
			if (munchie->frameCount >= 2)
				munchie->frameCount = 0;
			munchie->currentFrameTime = 0;
		}
		//Change Munchie sprite
		munchie->rect->X = munchie->rect->Width * munchie->frameCount;
	
 }
void Pacman::UpdateSpecial(SpecialMunchie* specialMunchie, int elapsedTime)
{
	specialMunchie->currentFrameTime += elapsedTime;
	if (specialMunchie->currentFrameTime > specialMunchie->cMunchieFrameTime)
	{
		specialMunchie->frameCount++;
		if (specialMunchie->frameCount >= 2)
			specialMunchie->frameCount = 0;
		specialMunchie->currentFrameTime = 0;
	}
	//Change Munchie sprite
	specialMunchie->rect->X = specialMunchie->rect->Width * specialMunchie->frameCount;
}

void Pacman::UpdatePacman(int elapsedTime)
	 {


		 //Frame Time
		 _pacman->currentFrameTime += elapsedTime;

		 if (_pacman->currentFrameTime > _pacman->cFrameTime)
		 {
			 _pacman->frame++;
			 if (_pacman->frame >= 2)
				 _pacman->frame = 0;

			 _pacman->currentFrameTime = 0;
		 }
		 //Change pacman sprite
		 _pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;

		 _pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
	 }

void Pacman::UpdateGhost(int elapsedTime)
{
		//Frame Time
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->currentFrameTime += elapsedTime;

		if (_ghosts[i]->currentFrameTime > _ghosts[i]->cFrametime)
		{
			_ghosts[i]->frame++;
			if (_ghosts[i]->frame >= 2)
				_ghosts[i]->frame = 0;

			_ghosts[i]->currentFrameTime = 0;
		}

		for (int i = 0; i < GHOSTCOUNT; i++)
		{
			//Changing ghost sprite
			_ghosts[i]->sourceRect->Y = _ghosts[i]->sourceRect->Height * _ghosts[i]->direction;

			_ghosts[i]->sourceRect->X = _ghosts[i]->sourceRect->Width * _ghosts[i]->frame;
		}

	}

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		if (_ghosts[i]->direction == 0)//Right
		{
			_ghosts[i]->position->X += _ghosts[i]->speed * elapsedTime;
		}
		else if (_ghosts[i]->direction == 1)//Left
		{
			_ghosts[i]->position->X -= _ghosts[i]->speed * elapsedTime;
		}
		else if (_ghosts[i]->direction == 2)//up
		{
			_ghosts[i]->position->Y -= _ghosts[i]->speed * elapsedTime;

		}
		else if (_ghosts[i]->direction == 3)//down
		{
			_ghosts[i]->position->Y += _ghosts[i]->speed * elapsedTime;
		}
	}
}

//Checks
void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
		Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
		//Pause Menu
		if (keyboardState->IsKeyDown(Input::Keys::P) && !_pkeyDown)
		{
			_pkeyDown = true;
			_paused = !_paused;
		}
		if (keyboardState->IsKeyUp(Input::Keys::P))
			_pkeyDown = false;

}

void Pacman::CheckViewPortCollision()
	{
		if (_pacman->position->X + _pacman->sourceRect->Width >= Graphics::GetViewportWidth())
				_pacman->position->X = 0;
		if (_pacman->position->X + (_pacman->sourceRect->Width - 32) < 0)
				_pacman->position->X = Graphics::GetViewportWidth() - 32;
		if (_pacman->position->Y + _pacman->sourceRect->Height >= Graphics::GetViewportHeight())
				_pacman->position->Y = 0;
		if (_pacman->position->Y + (_pacman->sourceRect->Height - 32) < 0)
				_pacman->position->Y = Graphics::GetViewportHeight() - 32;
	}

bool Pacman::CollisionCheck(int x1, int y1, int width1, int height1,
	int x2, int y2, int width2, int height2)
{
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;

	if (bottom1 < top2)
		return false;

	if (top1 > bottom2)
			return false;

	if (right1 < left2)
			return false;

	if (left1 > right2)
			return false;
	
		return true;
}

void Pacman::CheckGhostCollisions()
{
	//Collision with ninja
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
			_ghosts[i]->position->X, _ghosts[i]->position->Y, _ghosts[i]->sourceRect->Width, _ghosts[i]->sourceRect->Height))
		{
			_pacman->dead = true;
		}
	}
	//Ghost Patrol
	//enemy 1
	if (_ghosts[0]->position->X > 14 * 32 + 20)
	{
		_ghosts[0]->position->X = 14 * 32 + 18;
		_ghosts[0]->direction = 3;
	}
	else if (_ghosts[0]->position->X < 2 * 32 - 6)
	{
		_ghosts[0]->position->X = 2 * 32 - 4;
		_ghosts[0]->direction = 2;
	}
	else if (_ghosts[0]->position->Y > 5 * 32 + 10)
	{
		_ghosts[0]->position->Y = 5 * 32 + 8;
		_ghosts[0]->direction = 1;
	}
	else if (_ghosts[0]->position->Y < 1 * 32)
	{
		_ghosts[0]->position->Y = 1 * 32 + 2;
		_ghosts[0]->direction = 0;
	}
	//enemy 2
	if (_ghosts[1]->position->X > 30 * 32 + 20)
	{
		_ghosts[1]->position->X = 30 * 32 + 18;
		_ghosts[1]->direction = 2;
	}
	else if (_ghosts[1]->position->Y < 1 * 32)
	{
		_ghosts[1]->position->Y = 1 * 32 + 2;
		_ghosts[1]->direction = 1;
	}
	else if (_ghosts[1]->position->X < 23 * 32 + 20 )
	{
		_ghosts[1]->position->X = 23 * 32 + 20;
		_ghosts[1]->direction = 3;
	}
	else if (_ghosts[1]->position->Y > 6 * 32)
	{
		_ghosts[1]->position->Y = 6 * 32;
		_ghosts[1]->direction = 0;
	}
	//enemy 3
	if (_ghosts[2]->position->X > 13 * 32 + 20)
	{
		_ghosts[2]->position->X = 13 * 32 + 20;
		_ghosts[2]->direction = 1;
	}
	else if (_ghosts[2]->position->X < 2 * 32 + 20)
	{
		_ghosts[2]->position->X = 2 * 32 + 20;
		_ghosts[2]->direction = 0;
	}
	//enemy 4
	if (_ghosts[3]->position->X > 29 * 32 + 20)
	{
		_ghosts[3]->position->X = 29 * 32 + 20;
		_ghosts[3]->direction = 1;
	}
	else if (_ghosts[3]->position->X < 17 * 32 + 20)
	{
		_ghosts[3]->position->X = 17 * 32 + 20;
		_ghosts[3]->direction = 0;
	}
	//enemy 5
	 if (_ghosts[4]->position->X > 5 * 32 + 20)
	{
		_ghosts[4]->position->X = 5 * 32 + 18;
		_ghosts[4]->direction = 3;
	}
	 else if (_ghosts[4]->position->X < 34 )
	 {
		 _ghosts[4]->position->X = 34;
		 _ghosts[4]->direction = 2;
	 }
	 else if (_ghosts[4]->position->Y > 20 * 32 + 20)
	 {
		 _ghosts[4]->position->Y = 20 * 32 + 20;
		 _ghosts[4]->direction = 1;
	 }
	 else if (_ghosts[4]->position->Y < 15 * 32 + 20)
	 {
		 _ghosts[4]->position->Y = 15 * 32 + 20;
		 _ghosts[4]->direction = 0;
	 }
	
}

//Input
void Pacman::Input(int elapsedTime, Input::KeyboardState* kState, Input::MouseState* mState)
{
	float pacmanSpeed = _pacman->cSpeed * elapsedTime * _pacman->speedMultuplier;


	// Gets the current state of the keyboard and Mouse
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();


	
	// Checks if D key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::D))
	{
		_pacman->position->X += pacmanSpeed;
		_pacman->direction = 0;
		UpdatePacman(elapsedTime);
	}
	// Checks if A key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::A))
	{
		_pacman->position->X -= pacmanSpeed;
		_pacman->direction = 2;
		UpdatePacman(elapsedTime);
	}
	// Checks if W key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::W))
	{
		_pacman->position->Y -= pacmanSpeed;
		_pacman->direction = 3;
		UpdatePacman(elapsedTime);
	}
	// Checks if S key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::S))
	{
		_pacman->position->Y += pacmanSpeed;
		_pacman->direction = 1;
		UpdatePacman(elapsedTime);
	}

	if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		//Boost
		_pacman->speedMultuplier = 0.005f;
	}
	else
	{
		_pacman->speedMultuplier = 1.0f;
	}

	if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		_munchies[2]->position->X = mState->X;
		_munchies[2]->position->Y = mState->Y;
	}

	if (keyboardState->IsKeyDown(Input::Keys::R))
	{
		_pacman->dead = false;
			

	}
	
}

//Draw
void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Score:" << _pacman->scoreCount;

	SpriteBatch::BeginDraw(); // Starts Drawing
	
	 if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Red);
	}
	// Draws Pacman
	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); 

	}
	//Draws Munchie
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->rect);
	}
	//Draws SpecialMunchie
	for (int i = 0; i < SPECIALMUNCHIE; i++)
	{
		SpriteBatch::Draw(_specialMunchie[i]->texture, _specialMunchie[i]->position, _specialMunchie[i]->rect);
	}
	//Draws Blocks
	for (int i = 0; i < BLOCKCOUNT; i++)
	{
		SpriteBatch::Draw(_map[i]->texture, _map[i]->position, _map[i]->rectangle);
	}
	//Draws Enemies
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect);
	}
	
	 if (_pacman->dead)
	 {
		 SpriteBatch::Draw(_menu->deathscreen, _menu->rect, nullptr);
		
	 }

	if (_gameStarted)
	{
		

		SpriteBatch::Draw(_menu->startGame, _menu->startRectangle, nullptr);
		
	}
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Yellow);
	SpriteBatch::EndDraw(); // Ends Drawing
}
