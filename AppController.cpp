#include <string>
#include "AppController.h"
#include "SequenceController.h"
#include "MenuController.h"
#include "PasswordController.h"
#include "PrefaseController.h"
#include "GameController.h"
#include "PostfaseController.h"
#include "MortController.h"
#include "const.h"

AppController::AppController(int pMode)
{
	gameInfo = new GameInfo(pMode);
}

AppController::~AppController(void)
{
}

bool AppController::Init(void)
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) { return false; }

	drawManager = new DrawManager(gameInfo->modeGrafic);
	inputManager = new InputManager(gameInfo);
	musicManager = new MusicManager();

	if ( !drawManager->Init() ) { return false; }
	musicManager->Init();

	SDL_WM_SetCaption( APPLICATION_NAME, NULL );

	SDL_ShowCursor(0);

	return true;
}

void AppController::Go()
{
	if (!Init()) gameInfo->estado = ESTADO_SALIR;

	while ( gameInfo->estado != ESTADO_SALIR ) {
		switch ( gameInfo->estado ) {
			case ESTADO_SEQUENCIA:
				SequenceController *sequenceController;
				sequenceController = new SequenceController(drawManager, inputManager, musicManager);
				sequenceController->Go(gameInfo);
				delete sequenceController;
				break;
			case ESTADO_MENU:
				MenuController *menuController;
				menuController = new MenuController(drawManager, inputManager, musicManager);
				menuController->Go(gameInfo);
				delete menuController;
				break;
			case ESTADO_PASSWORD:
				PasswordController *passwordController;
				passwordController = new PasswordController(drawManager, inputManager, musicManager);
				passwordController->Go(gameInfo);
				delete passwordController;
				break;
			case ESTADO_PREFASE:
				PrefaseController *prefaseController;
				prefaseController = new PrefaseController(drawManager, inputManager, musicManager);
				prefaseController->Go(gameInfo);
				delete prefaseController;
				break;
			case ESTADO_JUEGO:
				GameController *gameController;
				gameController = new GameController(drawManager, inputManager, musicManager);
				gameController->Go(gameInfo);
				delete gameController;
				break;
			case ESTADO_POSTFASE:
				PostfaseController *postfaseController;
				postfaseController = new PostfaseController(drawManager, inputManager, musicManager);
				postfaseController->Go(gameInfo);
				delete postfaseController;
				break;
			case ESTADO_MORT:
				MortController *mortController;
				mortController = new MortController(drawManager, inputManager, musicManager);
				mortController->Go(gameInfo);
				delete mortController;
				break;
		}
	}

	Finalize();
}

void AppController::Finalize(void)
{
	delete drawManager;
	delete inputManager;
	delete musicManager;
	delete gameInfo;

	SDL_Quit();
}