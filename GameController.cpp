#include "GameController.h"
#include "const.h"
#include "fileManager.h"

GameController::GameController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager) {
	drawManager = p_drawManager;
	inputManager = p_inputManager;
	musicManager = p_musicManager;
}

GameController::~GameController(void) {
}

bool GameController::Init()
{
	char arxiuFondo[10] = "BKG00.GIF";
	arxiuFondo[4] = (gameInfo->fase % 10) + 48;
	fondo = drawManager->LoadBitmap(arxiuFondo);
	sprites = drawManager->LoadBitmap("sprites.gif", true);
	font1 = drawManager->LoadFont("fuente1.gif");
	cursor = drawManager->LoadFont("cursor.gif");
	puerta = drawManager->LoadFont("puerta.gif");
	marca = drawManager->LoadFont("marca.gif");
	menu = drawManager->LoadFont("menu.gif");

	aiguaProcesor = new AiguaProcesor(drawManager, gameInfo->fase);
	marcadorProcesor = new MarcadorProcesor(drawManager);

	CarregarMapa(gameInfo->fase);

	primerArounder = new ArounderProcesor(drawManager, marcadorProcesor, sprites, mapa, (xInicial*16)+8, (yInicial*16)+8, (xFinal*16)+8, (yFinal*16)+8);
	marcadorProcesor->numAroundersEixits ++;
	arounderSeleccionat = primerArounder;

	if ( ((gameInfo->fase+1) % 5) == 0) {
		musicManager->Load( "mus6.mp3");
	} else {
		musicManager->Load( "mus4.mp3");
	}

	return true;
}

void GameController::Go(GameInfo *pGameInfo)
{
	gameInfo = pGameInfo;

	bool salir = false;

	int mouseX = 0;
	int mouseY = 0;
	int nuevaAccion = -1;

	if ( !Init() ) { salir = true; gameInfo->estado = ESTADO_SALIR; }

	musicManager->Play(-1);
	drawManager->Blit(0,0, fondo);
	drawManager->Blit(0,0, mapa);
	drawManager->FadeIn();

	int arounderCount = SDL_GetTicks();
	startTicks = SDL_GetTicks();

	while ( !salir ) {

		PintarEscena();
		PintarCursor();
		drawManager->Flip();

		inputManager->Update();
		mouseX = inputManager->mouseX;
		mouseY = inputManager->mouseY;

		if (inputManager->Pausa() || inputManager->BotoPulsat(SDL_BUTTON_MIDDLE) || !inputManager->finestraActiva) {
			arounderCount = SDL_GetTicks() - arounderCount;
			salir = Pausa();
			arounderCount = SDL_GetTicks() - arounderCount;
		}

		if (inputManager->Menu()) {
			arounderCount = SDL_GetTicks() - arounderCount;
			salir = Menu();
			arounderCount = SDL_GetTicks() - arounderCount;
		}

		if (inputManager->eixir) {
			salir = true;
			gameInfo->estado = ESTADO_SALIR;
		}

		if (inputManager->BotoPulsat(SDL_BUTTON_RIGHT) && arounderSeleccionat != NULL) {
			arounderSeleccionat->AbortarAccio();
		}

		if (inputManager->BotoPulsat(SDL_BUTTON_LEFT)) {
			ArounderProcesor *nouSeleccionat = primerArounder->Seleccionar(mouseX, mouseY);
			if (nouSeleccionat != NULL) {
				arounderSeleccionat = nouSeleccionat;
			} else if (mouseY < 165 && arounderSeleccionat != NULL && arounderSeleccionat->accio == ACCIO_CAMINAR) {
				if (mouseX > arounderSeleccionat->X) {
					arounderSeleccionat->O = ORIENT_DRETA;
				} else {
					arounderSeleccionat->O = ORIENT_ESQUERRA;
				}
			}
			nuevaAccion = marcadorProcesor->Procesar(mouseX, mouseY);
		}

		if (inputManager->BotoSoltat(SDL_BUTTON_LEFT)) {
			if (nuevaAccion != -1 && arounderSeleccionat != NULL) {
				if (nuevaAccion == arounderSeleccionat->prevista) {
					arounderSeleccionat->prevista = arounderSeleccionat->accio;
				} else {
					arounderSeleccionat->prevista = nuevaAccion;
				}
			}
		}

		currentTicks = SDL_GetTicks() - startTicks;
		if( currentTicks >= marcadorProcesor->velocitat ) {
			startTicks = SDL_GetTicks();
			aiguaProcesor->Procesar();
			if (primerArounder != NULL) {
				primerArounder->Procesar();

				ArounderProcesor *actual = primerArounder;
				while (actual->siguiente != NULL) {
					actual = actual->siguiente;
				}
				
				while (actual != NULL) {
					if (actual->accio == ACCIO_MORT) {
						ArounderProcesor *borrar = actual;
						if (borrar->siguiente != NULL) {
							borrar->siguiente->anterior = borrar->anterior;
						}
						if (borrar->anterior != NULL) {
							borrar->anterior->siguiente = borrar->siguiente;
						}
						if (borrar == primerArounder) primerArounder = borrar->siguiente;
						if (borrar == arounderSeleccionat) arounderSeleccionat = NULL;
						actual = borrar->anterior;
						delete borrar;
					} else {
						actual = actual->anterior;
					}
				}
			}
		} 

		if ( (SDL_GetTicks() - arounderCount) >= marcadorProcesor->velocitat*58) {
			if (marcadorProcesor->numAroundersEixits < marcadorProcesor->numArounders) {
				if (primerArounder != NULL) {
					primerArounder->Afegir();
				} else {
					primerArounder = new ArounderProcesor(drawManager, marcadorProcesor, sprites, mapa, (xInicial*16)+8, (yInicial*16)+8, (xFinal*16)+8, (yFinal*16)+8);
				}
				marcadorProcesor->numAroundersEixits ++;
				arounderCount = SDL_GetTicks();
			}
		}

		if (marcadorProcesor->numAroundersArrivats + marcadorProcesor->numAroundersMorts == marcadorProcesor->numArounders) {
			if (marcadorProcesor->numAroundersArrivats >= marcadorProcesor->AroundersNec) {
				salir = true;
				gameInfo->estado = ESTADO_POSTFASE;
				gameInfo->fase++;
			} else {
				salir = true;
				gameInfo->estado = ESTADO_MORT;
			}
		}

		if (inputManager->BotoPulsat(SDL_BUTTON_MIDDLE)) {
			salir = true;
			gameInfo->estado = ESTADO_JUEGO;
			gameInfo->fase++;
		}
		/*if (arounderSeleccionat != NULL && arounderSeleccionat->prevista == ACCIO_SUICIDI2) {
			gameInfo->estado = ESTADO_MORT;
			salir = true;
		}*/

	}

	musicManager->FadeOut();
	drawManager->FadeOut();
	
	Finalize();
}

void GameController::PintarEscena() {
	int accio = -1;
	int prevista = -1;

	drawManager->Blit(0,0, fondo);
	drawManager->Blit(0,0, mapa);
	drawManager->Blit(xInicial*16, yInicial*16, puerta);
	drawManager->Blit(xFinal*16, yFinal*16, puerta);

	if (primerArounder != NULL) primerArounder->Pintar();

	aiguaProcesor->Pintar();

	if (arounderSeleccionat != NULL) {
		accio = arounderSeleccionat->accio;
		prevista = arounderSeleccionat->prevista;
	} else {
		accio = -1;
		prevista = -1;
	}
	marcadorProcesor->Pintar(font1, accio, prevista);

	if (arounderSeleccionat != NULL) {
		drawManager->Blit(arounderSeleccionat->X-3, arounderSeleccionat->Y-3, marca);
	}
}

void GameController::PintarCursor() {
	drawManager->Blit(inputManager->mouseX, inputManager->mouseY, cursor);
}

bool GameController::Pausa() {

	PintarEscena();
	drawManager->FadeOutGray();

	bool salir = false;
	bool salirJuego = false;
	
	while (!salir) {
		drawManager->DrawFadedBack();
		drawManager->Print(136,80, font1, 7, 5, "PAUSA");
		drawManager->Blit(inputManager->mouseX, inputManager->mouseY, cursor);
		drawManager->Flip();

		inputManager->Update();
		if (inputManager->eixir) {
			salir = true;
			gameInfo->estado = ESTADO_SALIR;
		}

		if (inputManager->Menu()) {
			salirJuego = Menu(true);
			salir = salirJuego;
		}

		if (inputManager->Pausa() || inputManager->BotoPulsat(SDL_BUTTON_MIDDLE)) salir = true;
	}
	
	drawManager->FadeInGray();

	return salirJuego;
}

bool GameController::Menu(bool bypass) {

	if (!bypass) {
		PintarEscena();
		drawManager->FadeOutGray();
	}

	bool salir = false;
	bool salirJuego = false;
	
	while (!salir) {
		drawManager->DrawFadedBack();
		drawManager->Blit(97, 52, menu);
		drawManager->Print(129,60, font1, 7, 5, "CONTINUAR");
		drawManager->Print(129,71, font1, 7, 5, "REINICIAR");
		drawManager->Print(112,82, font1, 7, 5, "MENU PRINCIPAL");
		drawManager->Print(143,93, font1, 7, 5, "EIXIR");
		drawManager->Blit(inputManager->mouseX, inputManager->mouseY, cursor);
		drawManager->Flip();

		inputManager->Update();
		if (inputManager->eixir) {
			salir = true;
			gameInfo->estado = ESTADO_SALIR;
		}
		if (inputManager->Menu()) salir = true;

		if (inputManager->BotoPulsat(SDL_BUTTON_LEFT)) {
			if (inputManager->mouseX >= 97 && inputManager->mouseX <= 223) {
				if (inputManager->mouseY >= 60 && inputManager->mouseY <= 65) {
					salir = true;
					//salirJuego = true;
					//gameInfo->fase++;
				}
				if (inputManager->mouseY >= 71 && inputManager->mouseY <= 76) {
					salirJuego = true;
					salir = true;
				}
				if (inputManager->mouseY >= 82 && inputManager->mouseY <= 87) {
					salirJuego = true;
					salir = true;
					gameInfo->estado = ESTADO_MENU;
				}
				if (inputManager->mouseY >= 93 && inputManager->mouseY <= 98) {
					salirJuego = true;
					salir = true;
					gameInfo->estado = ESTADO_SALIR;
				}
			}
		}
	}
	
	if (!bypass) {
		drawManager->FadeInGray();
	}

	return salirJuego;
}

void GameController::CarregarMapa(int numMapa)
{
	int filesize = 0;
	char *buffer = GetBufferFromResource("MAPES.BAL", &filesize);

	int punter = numMapa * 212;

	int tileSet = buffer[punter++];
	punter++;
	marcadorProcesor->orientacioInicial = buffer[punter++];
	marcadorProcesor->numArounders = buffer[punter++];
	marcadorProcesor->AroundersNec = buffer[punter++];

	marcadorProcesor->numParar = buffer[punter++];
	marcadorProcesor->numCavar = buffer[punter++];
	marcadorProcesor->numEscalar = buffer[punter++];
	marcadorProcesor->numPerforar = buffer[punter++];
	marcadorProcesor->numEscalera = buffer[punter++];
	marcadorProcesor->numPasarela = buffer[punter++];
	marcadorProcesor->numCorda = buffer[punter++];

	SDL_Surface *tiles = drawManager->LoadBitmap("tiles.gif", true);
	mapa = drawManager->LoadBitmap("black.gif", true);

	unsigned char tileActual;
	SDL_Rect clip;
	clip.w = 16;
	clip.h = 16;

	for (int x=0; x<20; x++) {
		for (int y=0; y<10; y++) {
			tileActual = buffer[punter++];
			switch (tileActual) {
				case 255:
					break;
				case 254:
					xFinal = x;
					yFinal = y;
					break;
				case 253:
					xInicial = x;
					yInicial = y;
					break;
				default:
					clip.x = tileActual*16;
					clip.y = tileSet*16;
					drawManager->apply_surface(x*16, y*16, tiles, mapa, &clip);
					break;
			}
		}
	}

	SDL_FreeSurface(tiles);
	SDL_Surface *marcador = drawManager->LoadBitmap("marcador.gif", false);
	drawManager->apply_surface(0, 165, marcador, mapa);
	SDL_FreeSurface(marcador);
	
	drawManager->Print(188,188, font1, 7, 5, formatejar(gameInfo->fase+1), mapa);

	drawManager->Print(7,188, font1, 7, 5, "XX", mapa);
	drawManager->Print(135,188, font1, 7, 5, "XX", mapa);
	drawManager->Print(151,188, font1, 7, 5, "XX", mapa);

	drawManager->Print(224,171, font1, 7, 5, "ACTIUS", mapa);
	drawManager->Print(224,177, font1, 7, 5, "TOTAL", mapa);
	drawManager->Print(224,183, font1, 7, 5, "NECESSARIS", mapa);
	drawManager->Print(224,189, font1, 7, 5, "ARRIVATS", mapa);

}


void GameController::Finalize(void)
{
	SDL_FreeSurface(font1);
	SDL_FreeSurface(mapa);
	SDL_FreeSurface(fondo);
	SDL_FreeSurface(marca);
	SDL_FreeSurface(sprites);
	SDL_FreeSurface(cursor);
	SDL_FreeSurface(puerta);
	SDL_FreeSurface(menu);
	delete aiguaProcesor;
	delete marcadorProcesor;

	if (primerArounder != NULL) {
		while (primerArounder->siguiente != NULL) {
			delete primerArounder->siguiente;
		}
		delete primerArounder;
	}
}

char *GameController::formatejar(int numero) {
	char *resultat;
	resultat = (char *) malloc(3);

	if (numero > 9) {
			resultat[0] = (numero / 10) + 48;
			resultat[1] = (numero % 10) + 48;
	} else {
			resultat[0] = 48;
			resultat[1] = (numero % 10) + 48;
	}

	resultat[2] = '\0';

	return resultat;
}
