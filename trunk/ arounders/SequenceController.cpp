#include "SequenceController.h"
#include "const.h"
#include "fileManager.h"

SequenceController::SequenceController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager) {
	drawManager = p_drawManager;
	inputManager = p_inputManager;
	musicManager = p_musicManager;
}

SequenceController::~SequenceController(void) {
}

bool SequenceController::Init() {
	return true;
}

void SequenceController::Go(GameInfo *pGameInfo) {
	bool salir = false;
	int pantalla = 0;

	gameInfo = pGameInfo;

	if (gameInfo->fase % 5 == 0 || gameInfo->fase < 0) {
		musicManager->Load( "mus3.mid");
		musicManager->Play(-1);
		switch (gameInfo->fase) {
			case -1:
				SequenciaInicial();
				break;
			case 0:
				Sequencia00();
				break;
			case 5:
				Sequencia05();
				break;
			case 10:
				Sequencia10();
				break;
			case 15:
				Sequencia15();
				break;
			case 20:
				Sequencia20();
				break;
			case 25:
				Sequencia25();
				break;
			case 30:
				Sequencia30();
				break;
		}
	}
	drawManager->FadeOut();

	gameInfo->estado = ESTADO_PREFASE;
	if (gameInfo->fase == -1) gameInfo->estado = ESTADO_MENU;
	if (gameInfo->fase == 30) gameInfo->estado = ESTADO_CREDITS;
}

void SequenceController::SequenciaInicial() {
	FadeIn("intro01.GIF");
	Print(39, 149, -1, "8 DEL MATI");
	Esperar(1000);
	Print(39, 156, -1, "LABORATORI DE FFI");
	Esperar(4000);
	FadeOut();

	FadeIn("intro02.GIF");
	Print(39, 172, -1, "EL PROFESOR BACTERIOL ESTA");
	Print(39, 179, -1, "PREPARANT EL SEU NOU INVENT...");
	Esperar(4000);
	FadeOut();

	FadeIn("intro03.GIF");
	Print(119, 19, drawManager->color_rojo, "\"COGEMOL EL INDIVIDUOL");
	Print(119, 26, drawManager->color_rojo, "Y PULSANDOL EL BOTOL...");
	Esperar(3000);
	Show("intro03.GIF");
	Print(120, 19, drawManager->color_rojo, "...CONSEGUIMOL VARIOL");
	Print(120, 26, drawManager->color_rojo, "INDIVIDUOL...\"");
	Esperar(4000);
	FadeOut();

	FadeIn("intro04.GIF");
	Print(79, 179, -1, "PERO ALGUN DESAPRENSIU...");
	Esperar(2000);
	Show("intro04.GIF");
	Print(109, 39, drawManager->color_verde, "\"EY! ME ANE A FERME");
	Print(109, 46, drawManager->color_verde, "UN CORTAET...\"");
	Esperar(3000);
	FadeOut();

	FadeIn("intro05.GIF");
	Print(124, 154, -1, "ERA BALOO!!!");
	Esperar(1000);
	Print(124, 161, -1, "I HA ENXUFAT LA MAQUINA!");
	Esperar(3000);
	FadeOut();

	FadeIn("intro06.GIF");
	Print(109, 9, drawManager->color_rojo, "\"LA CAGAMOL!!!");
	Print(109, 16, drawManager->color_rojo, "AHORAL QUE HAGOL!\"");
	Esperar(5000);
	FadeOut();

}

void SequenceController::Sequencia00() {
}

void SequenceController::Sequencia05() {
}

void SequenceController::Sequencia10() {
}

void SequenceController::Sequencia15() {
}

void SequenceController::Sequencia20() {
}

void SequenceController::Sequencia25() {
}

void SequenceController::Sequencia30() {
}

bool SequenceController::Esperar(Uint32 temps) {
	bool seguirIntro = true;
	bool salir = false;
	Uint32 startTicks = SDL_GetTicks();

	while ( !salir ) {
		inputManager->Update();

		if (inputManager->eixir) {
			gameInfo->estado = ESTADO_SALIR;
			salir = true;
			seguirIntro = false;
		}

		if (inputManager->cualquierTecla || inputManager->BotoPulsat(SDL_BUTTON_LEFT)) {
			salir = true;
			seguirIntro = false;
		}

		if (SDL_GetTicks()-startTicks > temps) {
			salir = true;
		}
	}
	return seguirIntro;
}

void SequenceController::FadeIn(char *archivo) {
	fondo = drawManager->LoadBitmap( archivo);
	drawManager->Blit(0,0, fondo);
	drawManager->FadeIn();
	SDL_FreeSurface(fondo);
}

void SequenceController::Show(char *archivo) {
	fondo = drawManager->LoadBitmap( archivo);
	drawManager->Blit(0,0, fondo);
	drawManager->Flip();
	SDL_FreeSurface(fondo);
}

void SequenceController::Print(int x, int y, Uint32 color, char *texto) {
	font = drawManager->LoadFont( "fuente2.gif", color);
	drawManager->Print(x, y, font, 7, 6, texto);
	drawManager->Flip();
	SDL_FreeSurface(font);
}

void SequenceController::FadeOut() {
	drawManager->FadeOut();
}

void SequenceController::Finalize(void) {

}
