#include "MenuController.h"
#include "const.h"

MenuController::MenuController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager) {
	drawManager = p_drawManager;
	inputManager = p_inputManager;
	musicManager = p_musicManager;
}

MenuController::~MenuController(void) {
}

bool MenuController::Init() {
	fondo = drawManager->LoadBitmap("menuprin.gif");
	cursor = drawManager->LoadFont("cursor.gif");
	if (!musicManager->Sonant()) {
		musicManager->Load( "mus3.mp3");
		musicManager->Play(-1);
	}
	return true;
}

void MenuController::Go(GameInfo *gameInfo) {
	bool salir = false;

	if ( !Init() ) { salir = true; gameInfo->estado = ESTADO_SALIR; }

	drawManager->Blit(0,0, fondo);
	drawManager->FadeIn();

	while ( !salir ) {
		inputManager->Update();

		drawManager->Blit(0,0, fondo);
		drawManager->Blit(inputManager->mouseX, inputManager->mouseY, cursor);
		drawManager->Flip();

		if (inputManager->eixir) {
			gameInfo->estado = ESTADO_SALIR;
			salir = true;
		}

		if (inputManager->BotoPulsat(SDL_BUTTON_LEFT)) {
			if (inputManager->mouseX >= 200 && 
				inputManager->mouseY >= 100 && 
				inputManager->mouseX <= 270 && 
				inputManager->mouseY <= 120 ) {
					gameInfo->estado = ESTADO_SEQUENCIA;
					gameInfo->fase = 0;
					salir = true;
			}
			if (inputManager->mouseX >= 175 && 
				inputManager->mouseY >= 125 && 
				inputManager->mouseX <= 290 && 
				inputManager->mouseY <= 145 ) {
					gameInfo->estado = ESTADO_PASSWORD;
					gameInfo->fase = 0;
					salir = true;
			}
			if (inputManager->mouseX >= 200 && 
				inputManager->mouseY >= 150 && 
				inputManager->mouseX <= 265 && 
				inputManager->mouseY <= 170 ) {
					gameInfo->estado = ESTADO_SALIR;
					salir = true;
			}
		}
	}
	if (gameInfo->estado == ESTADO_SALIR) musicManager->FadeOut();
	drawManager->FadeOut();
}

void MenuController::Finalize(void) {
	SDL_FreeSurface(fondo);
	SDL_FreeSurface(cursor);
}
