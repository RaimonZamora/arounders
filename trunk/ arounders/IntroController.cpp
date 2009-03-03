#include "IntroController.h"
#include "const.h"
#include "fileManager.h"

IntroController::IntroController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager) {
	drawManager = p_drawManager;
	inputManager = p_inputManager;
	musicManager = p_musicManager;
}

bool IntroController::Init() {
	musicManager->Load( "mus1.mid");
	this->fondo = drawManager->LoadBitmap("jailgames.gif");
	return true;
}

void IntroController::Go(GameInfo *gameInfo) {
	bool salir = false;
	int pantalla = 0;

	if ( !this->Init() ) { salir = true; gameInfo->estado = ESTADO_SALIR; }

	musicManager->Play(1);
	drawManager->Blit(0,0, this->fondo);
	drawManager->FadeIn();

	int startTicks = SDL_GetTicks();

	while ( !salir ) {
		inputManager->Update();

		drawManager->Blit(0,0, this->fondo);
		drawManager->Flip();

		if (inputManager->eixir) {
			gameInfo->estado = ESTADO_SALIR;
			salir = true;
		}

		if (inputManager->cualquierTecla || inputManager->BotoPulsat(SDL_BUTTON_LEFT) || SDL_GetTicks()-startTicks > 4000) {
			if (pantalla == 0) {
				musicManager->FadeOut();
				drawManager->FadeOut();
				SDL_FreeSurface(this->fondo);
				this->fondo = drawManager->LoadBitmap("THX.GIF");
				musicManager->Load( "mus2.mid");
				musicManager->Play(1);
				drawManager->Blit(0,0, this->fondo);
				drawManager->FadeIn();
				pantalla++;
				startTicks = SDL_GetTicks();
			} else {
				gameInfo->estado = ESTADO_SEQUENCIA;
				salir = true;
			}
		}
	}
	musicManager->FadeOut();
	drawManager->FadeOut();
}

void IntroController::Finalize(void) {
	SDL_FreeSurface(this->fondo);
}
