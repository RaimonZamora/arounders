#pragma once
#include "SDL/SDL.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "MusicManager.h"
#include "GameInfo.h"

class AppController
{
public:
	AppController(int pMode);
	~AppController(void);

	void Go();

private:
	bool Init(void);
	void Finalize(void);

	DrawManager *drawManager;
	InputManager *inputManager;
	MusicManager *musicManager;
	GameInfo *gameInfo;

};
