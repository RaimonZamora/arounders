#include "MortController.h"
#include "const.h"

MortController::MortController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager) {
	drawManager = p_drawManager;
	inputManager = p_inputManager;
	musicManager = p_musicManager;
}

MortController::~MortController(void) {
}

bool MortController::Init() {
	fondo = drawManager->LoadBitmap("mort.gif");
	cursor = drawManager->LoadFont("cursor.gif");
	musicManager->Load( "mus5.mp3");

	return true;
}

void MortController::Go(GameInfo *gameInfo) {
	bool salir = false;

	if ( !Init() ) { salir = true; gameInfo->estado = ESTADO_SALIR; }

	musicManager->Play(-1);
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
			if (inputManager->mouseX >= 100 && 
				inputManager->mouseY >= 50 && 
				inputManager->mouseX <= 210 && 
				inputManager->mouseY <= 70 ) {
					gameInfo->estado = ESTADO_PREFASE;
					salir = true;
			}
			if (inputManager->mouseX >= 120 && 
				inputManager->mouseY >= 72 && 
				inputManager->mouseX <= 190 && 
				inputManager->mouseY <= 95 ) {
					gameInfo->estado = ESTADO_MENU;
					salir = true;
			}
			if (inputManager->mouseX >= 120 && 
				inputManager->mouseY >= 95 && 
				inputManager->mouseX <= 190 && 
				inputManager->mouseY <= 115 ) {
					gameInfo->estado = ESTADO_SALIR;
					salir = true;
			}
		}
	}
	drawManager->FadeOut();
}

void MortController::Finalize(void) {
	SDL_FreeSurface(fondo);
	SDL_FreeSurface(cursor);
}
