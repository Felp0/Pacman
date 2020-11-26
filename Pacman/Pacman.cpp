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
	//Map
	for (int i = 0; i < 768; i++) {
		{
			_map[i] = new Map();
		
		}
	}
	
	_paused = false;
	_pkeyDown = false;
	_gameStarted = true;
	_pacman->direction = 0;
	_pacman->currentFrameTime = 0;
	_pacman->frame = 0;
	_pacman->speedMultuplier = 1.0f;
	

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
	
	for (int nCount = 0; nCount < MUNCHIECOUNT; nCount++)
	{
		delete _munchies[nCount]->position;
		delete _munchies[nCount]->rect;
		delete _munchies[nCount];
		
	}
	delete[] _munchies;
	
}

void Pacman::LoadContent()
{
	//Set StartMenu Paramters
	_menu->startGame = new Texture2D();
	_menu->startGame->Load("Textures/Menu.png", false);
	_menu->startRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->startString1 = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 3.0f);
	// Set PauseMenu Paramters
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load munchie
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png", false);

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = munchieTex;
		_munchies[i]->texture->Load("Textures/Munchie.png", true);
		_munchies[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_munchies[i]->rect = new Rect(0.0f, 0.0f, 12, 12);
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
	for (int i = 0,j = 0,k = 0; i < 768; i++) 
	{
			
			if (aMap[i] == 1) 
			{
				_map[k]->position = new Vector2((i%32)*32, (i/32)*32);
				k++;
			}
			if (aMap[i] == 2)
			{
				_munchies[j]->position = new Vector2((i % 32) * 32, (i / 32) * 32);
				j++;

			}
			

	}
	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	
}

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

			//Check functions
			CheckViewPortCollision();

			//Sprite functions
			UpdatePacman(elapsedTime);


			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				UpdateMunchie(_munchies[i], elapsedTime);
				if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
					_munchies[i]->position->X, _munchies[i]->position->Y, _munchies[i]->rect->Width, _munchies[i]->rect->Height))
				{
					_munchies[i]->position->X = -100;
					_munchies[i]->position->Y = -100;
				}
			}

			for(int i = 0; i < BLOCKCOUNT; i++)
			{
				if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
					_map[i]->position->X, _map[i]->position->Y, _map[i]->rectangle->Width, _map[i]->rectangle->Height))
				{
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

void Pacman::UpdatePacman(int elapsedTime)
	 {
		 //Change pacman sprite
		 _pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;

		 _pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;


		 //Frame Time
		 _pacman->currentFrameTime += elapsedTime;

		 if (_pacman->currentFrameTime > _pacman->cFrameTime)
		 {
			 _pacman->frame++;
			 if (_pacman->frame >= 2)
				 _pacman->frame = 0;

			 _pacman->currentFrameTime = 0;
		 }
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

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	if (_gameStarted)
	{
		std::stringstream startStream;
		startStream << "MENU";

		SpriteBatch::Draw(_menu->startGame, _menu->startRectangle, nullptr);
		SpriteBatch::DrawString(startStream.str().c_str(), _menu->startString1, Color::Blue);
	}
	
	 if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Red);
	}

	SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->rect);
	}

	for (int i = 0; i < BLOCKCOUNT; i++)
	{
		SpriteBatch::Draw(_map[i]->texture, _map[i]->position, _map[i]->rectangle);
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
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
	}
	// Checks if A key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::A))
	{
		_pacman->position->X -= pacmanSpeed;
		_pacman->direction = 2;
	}
	// Checks if W key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::W))
	{
		_pacman->position->Y -= pacmanSpeed;
		_pacman->direction = 3;
	}
	// Checks if S key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::S))
	{
		_pacman->position->Y += pacmanSpeed;
		_pacman->direction = 1;
	}

	if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		//Boost
		_pacman->speedMultuplier = 5.0f;
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
		_munchies[3]->position->X = rand() % Graphics::GetViewportHeight();
		_munchies[3]->position->Y = rand() % Graphics::GetViewportWidth();
	}
}