#include "ArounderProcesor.h"

ArounderProcesor::ArounderProcesor(DrawManager *pDrawManager, MarcadorProcesor *pMarcadorProcesor, SDL_Surface *pSprites, SDL_Surface *pMapa, int pxInicial, int pyInicial, int pxFinal, int pyFinal )
{
	drawManager = pDrawManager;
	marcadorProcesor = pMarcadorProcesor;
	sprites = pSprites;
	mapa = pMapa;
	pixels = (Uint32 *)mapa->pixels;
	explosio = drawManager->LoadMask("explosio.gif");

	xInicial = pxInicial;
	yInicial = pyInicial;
	xFinal = pxFinal;
	yFinal = pyFinal;
	
	X = xInicial;
	Y = yInicial;
	O = marcadorProcesor->orientacioInicial;
	frame = 0;
	frameX = 0;
	frameY = 0;
	altura = 0;

	accio = ACCIO_CAMINAR;
	prevista = ACCIO_CAMINAR;

	clip.w = 8;
	clip.h = 8;

	siguiente = NULL;
	anterior = NULL;

	colorEscalo = SDL_MapRGB( mapa->format, 60, 0, 60 );
	colorCorda = SDL_MapRGB( mapa->format, 255, 251, 194 );
}

ArounderProcesor::~ArounderProcesor(void)
{
	if (anterior != NULL) anterior->siguiente = siguiente;
	if (siguiente != NULL) siguiente->anterior = anterior;

	SDL_FreeSurface(explosio);
}

ArounderProcesor *ArounderProcesor::Afegir() {
	if (siguiente != NULL) {
		return siguiente->Afegir();
	} else {
		siguiente = new ArounderProcesor(drawManager, marcadorProcesor, sprites, mapa, xInicial, yInicial, xFinal, yFinal);
		siguiente->anterior = this;
		return siguiente;
	}
}

void ArounderProcesor::Pintar() {
	clip.y = frameY;
	clip.x = frameX*8;
	drawManager->Blit(X, Y, sprites, &clip);

	if (siguiente != NULL) siguiente->Pintar();
}

ArounderProcesor *ArounderProcesor::Seleccionar(int mouseX, int mouseY) {
	if (X <= mouseX && (X+8) >= mouseX && Y <= mouseY && (Y+8) >= mouseY) {
		return this;
	} else {
		if (siguiente != NULL) {
			return siguiente->Seleccionar(mouseX, mouseY);
		} else {
			return NULL;
		}
	}
}

void ArounderProcesor::Procesar() {
	
	switch(accio) {
		case ACCIO_CAMINAR:
			procesarCaminar();
			break;
		case ACCIO_CAURE:
			procesarCaure();
			break;
		case ACCIO_CAVAR:
			procesarCavar();
			break;
		case ACCIO_ESCALAR:
			procesarEscalar();
			break;
		case ACCIO_PERFORAR:
			procesarPerforar();
			break;
		case ACCIO_ESCALERA:
			procesarEscalera();
			break;
		case ACCIO_PASARELA:
			procesarPasarela();
			break;
		case ACCIO_CORDA:
			procesarCorda();
			break;
		case ACCIO_PUJARCORDA:
			procesarPujarCorda();
			break;
		case ACCIO_BAIXARCORDA:
			procesarBaixarCorda();
			break;
		case ACCIO_SUICIDI:
			procesarSuicidi();
			break;
	}

	if (X == xFinal && Y == yFinal) initArrivat();

	if (siguiente != NULL) siguiente->Procesar();
}

void ArounderProcesor::AbortarAccio() {
	if (accio == ACCIO_ESCALAR) {
		if (O == ORIENT_DRETA) X=X-5; else X=X+5;
	}
	if (accio != ACCIO_CAMINAR && accio != ACCIO_CAURE && accio != ACCIO_PUJARCORDA && accio != ACCIO_BAIXARCORDA) {
		accio = ACCIO_CAMINAR;
	}
}

void ArounderProcesor::initMort() {
	accio = ACCIO_MORT;
	drawManager->apply_surface(X-9, Y-12, explosio, mapa);
	marcadorProcesor->numAroundersMorts++;
}

void ArounderProcesor::initArrivat() {
	accio = ACCIO_MORT;
	marcadorProcesor->numAroundersArrivats++;
}

void ArounderProcesor::initCaminar() {
	frame = 0;
	accio = ACCIO_CAMINAR;
	procesarCaminar();
}

void ArounderProcesor::initCaure() {
	frame = 0;
	accio = ACCIO_CAURE;
	altura = 0;
	procesarCaure();
}

void ArounderProcesor::initParar() {
	if (marcadorProcesor->numParar > 0) {
		frame = 0;
		accio = ACCIO_PARAR;
		prevista = ACCIO_CAMINAR;
		marcadorProcesor->numParar--;
		procesarParar();
	} else {
		frame = 0;
		accio = ACCIO_CAMINAR;
		prevista = ACCIO_CAMINAR;
		procesarCaminar();
	}
}

void ArounderProcesor::initCavar() {
	if (marcadorProcesor->numCavar > 0) {
		frame = 0;
		accio = ACCIO_CAVAR;
		prevista = ACCIO_CAMINAR;
		marcadorProcesor->numCavar--;
		procesarCavar();
	} else {
		frame = 0;
		accio = ACCIO_CAMINAR;
		prevista = ACCIO_CAMINAR;
		procesarCaminar();
	}
}

void ArounderProcesor::initEscalar() {
	if (marcadorProcesor->numEscalar > 0) {
		frame = 0;
		accio = ACCIO_ESCALAR;
		prevista = ACCIO_CAMINAR;
		marcadorProcesor->numEscalar--;
		if (O == ORIENT_DRETA) {
			X = X + 5;
		} else {
			X = X - 5;
		}
		procesarEscalar();
	} else {
		frame = 0;
		accio = ACCIO_CAMINAR;
		prevista = ACCIO_CAMINAR;
		procesarCaminar();
	}
}

void ArounderProcesor::initPerforar() {
	if (marcadorProcesor->numPerforar > 0) {
		frame = 0;
		accio = ACCIO_PERFORAR;
		prevista = ACCIO_CAMINAR;
		marcadorProcesor->numPerforar--;
		procesarPerforar();
	} else {
		frame = 0;
		accio = ACCIO_CAMINAR;
		prevista = ACCIO_CAMINAR;
		procesarCaminar();
	}
}

void ArounderProcesor::initEscalera() {
	frame = 0;
	accio = ACCIO_ESCALERA;
	prevista = ACCIO_CAMINAR;
	procesarEscalera();
}

void ArounderProcesor::initPasarela() {
	frame = 0;
	accio = ACCIO_PASARELA;
	prevista = ACCIO_CAMINAR;
	procesarPasarela();
}

void ArounderProcesor::initCorda() {
	if (marcadorProcesor->numCorda > 0) {
		frame = 0;
		accio = ACCIO_CORDA;
		prevista = ACCIO_CAMINAR;
		marcadorProcesor->numCorda--;
		procesarCorda();
		altura = 0;
	} else {
		frame = 0;
		accio = ACCIO_CAMINAR;
		prevista = ACCIO_CAMINAR;
		procesarCaminar();
	}
}

void ArounderProcesor::initPujarCorda() {
	frame = 0;
	accio = ACCIO_PUJARCORDA;
	if (O == ORIENT_DRETA) {
		X = X + 3;
	} else {
		X = X - 3;
	}
	procesarPujarCorda();
}

void ArounderProcesor::initBaixarCorda() {
	frame = 0;
	accio = ACCIO_BAIXARCORDA;
	if (O == ORIENT_DRETA) {
		X = X + 2;
	} else {
		X = X - 2;
	}
	procesarBaixarCorda();
}

void ArounderProcesor::initSuicidi() {
	frame = 0;
	accio = ACCIO_SUICIDI;
	prevista = accio;
	procesarSuicidi();
}


void ArounderProcesor::procesarCaminar() {
	const int frames[4] = {3,4,3,5};
	frame++;
	if (frame >= 4) frame = 0;
	frameX = frames[frame];
	frameY = O;

	if (blockCaure()) {
		initCaure();
	} else {
		switch ( prevista ) {
			case ACCIO_PARAR:
				initParar();
				break;
			case ACCIO_PERFORAR:
				initPerforar();
				break;
			case ACCIO_ESCALERA:
				initEscalera();
				break;
			case ACCIO_SUICIDI:
				initSuicidi();
				break;
			default:
				if (blockCordaPujar()) {
					initPujarCorda();
				} else if (blockCaminar()) {
					if (prevista == ACCIO_CAVAR) {
						initCavar();
					} else if (prevista == ACCIO_ESCALAR) {
						initEscalar();
					} else {
						O = O ^ ORIENT_ESQUERRA;
					}
				} else {
					if (blockArounder()) {
						O = O ^ ORIENT_ESQUERRA;
					} else {
						if (blockPrecipici() && (prevista == ACCIO_PASARELA || prevista == ACCIO_CORDA)) {
							if (prevista == ACCIO_PASARELA) {
								initPasarela();
							} else {
								initCorda();
							}
						} else {
							if (blockCordaBaixar()) {
								initBaixarCorda();
							} else if (blockCordaPujar()) {
								initPujarCorda();
							} else {
								if (pujarEscalo()) Y--;
								X+= ((((O ^ 8) << 1) >> 3) - 1);
								if (baixarEscalo()) Y++;
							}
						}
					}
				}
		}
	}
}

void ArounderProcesor::procesarCaure() {
	frame = 0;
	frameX = 11;
	frameY = O;

	if (!blockCaure()) {
		if (altura >= 32) {
			initMort();
		} else {
			initCaminar();
		}
	} else {
		if (Y > 151) {
			initMort();
		} else {
			Y++;
			altura++;
		}
	}
}

void ArounderProcesor::procesarParar() {
	frame = 0;
	frameX = 2;
	frameY = 8;
}

void ArounderProcesor::procesarCavar() {
	const int frames[3] = {6,7,8};
	frame++;
	if (frame == 3) frame = 0;
	frameX = frames[frame];
	frameY = O;

	if (blockCaure()) {
		initCaure();
	} else {
		if (!blockCaminar()) {
			initCaminar();
		} else {
			if (frame == 2) {
				doCavar();
				if (O == ORIENT_DRETA) {
					X++;
				} else {
					X--;
				}
			}
		}
	}
}

void ArounderProcesor::procesarEscalar() {
	const int frames[2] = {9,10};
	frame++;
	if (frame == 2) frame = 0;
	frameX = frames[frame];
	frameY = O;

	if (!blockParet(5)) {
		if (O == ORIENT_DRETA) {
			X = X - 1;
		} else {
			X = X + 1;
		}
		Y=Y-1;
		initCaminar();
	} else {
		if (blockTecho(5)) {
			if (O == ORIENT_DRETA) {
				X = X - 5;
			} else {
				X = X + 5;
			}
			initCaminar();
		} else {
			Y--;
		}
	}
}

void ArounderProcesor::procesarPerforar() {
	const int frames[2] = {0,1};
	frame++;
	if (frame == 2) frame = 0;
	frameX = frames[frame];
	frameY = 8;

	if (blockCaure()) {
		initCaure();
	} else {
		if (frame == 1) {
			doPerforar();
			Y++;
		}
	}
}

void ArounderProcesor::procesarEscalera() {
	const int frames[5] = {12,13,14,15,4};
	frame++;
	if (frame == 5) frame = 0;
	frameX = frames[frame];
	frameY = O;

	if (blockParet() || blockTecho() || marcadorProcesor->numEscalera == 0) {
		initCaminar();
	} else {
		if (frame == 3) {
			doEscalera();
			marcadorProcesor->numEscalera--;
		}
		if (frame == 4) {
			Y--;
			if (O == ORIENT_DRETA) {
				X++;
			} else {
				X--;
			}
		}
	}
}

void ArounderProcesor::procesarPasarela() {
	const int frames[5] = {12,13,14,15,4};
	frame++;
	if (frame == 5) frame = 0;
	frameX = frames[frame];
	frameY = O;

	if (blockParet() || !blockPrecipiciPasarela() || marcadorProcesor->numPasarela == 0) {
		initCaminar();
	} else {
		if (frame == 3) {
			doEscalera(1);
			marcadorProcesor->numPasarela--;
		}
		if (frame == 4) {
			if (O == ORIENT_DRETA) {
				X=X+2;
			} else {
				X=X-2;
			}
		}
	}
}

void ArounderProcesor::procesarCorda() {
	const int frames[4] = {12,13,14,15};
	if (frame < 3) frame++;
	frameX = frames[frame];
	frameY = O;

	if (frame == 3 ) {
		if (!blockNovaCorda()) {
			initCaminar();
		} else {
			doCorda();
			altura++;
		}
	}

}

void ArounderProcesor::procesarPujarCorda() {
	const int frames[2] = {9,10};
	frame++;
	if (frame == 2) frame = 0;
	frameX = frames[frame];
	frameY = O;

	if (!blockCordaPujar(3)) {
		Y = Y - 2;
		initCaminar();
	} else {
		if (blockTecho(3)) {
			if (O == ORIENT_DRETA) {
				X = X - 3;
			} else {
				X = X + 3;
			}
			initCaminar();
		} else {
			Y--;
		}
	}
}

void ArounderProcesor::procesarBaixarCorda() {
	const int frames[2] = {9,10};
	frame++;
	if (frame == 2) frame = 0;
	frameX = frames[frame];
	frameY = ORIENT_ESQUERRA - O;

	if (!blockCordaBaixar(2)) {
		initCaminar();
	} else {
		if (!blockCaure(3)) {
			if (O == ORIENT_DRETA) {
				X = X - 2;
			} else {
				X = X + 2;
			}
			initCaminar();
		} else {
			Y++;
		}
	}
}

void ArounderProcesor::procesarSuicidi() {
	const int frames[5] = {0,1,2,1,2};
	frameX = frames[frame];
	frameY = 0;

	if (frame == 4) initMort();
	frame++;
}

bool ArounderProcesor::blockCaure(int desfase) {
	if ( O == ORIENT_ESQUERRA) desfase = -desfase;
	return ( (get_pixel(X+desfase  , Y+8) == 0 || get_pixel(X+desfase  , Y+8) == colorCorda) &&
		     (get_pixel(X+desfase+1, Y+8) == 0 || get_pixel(X+desfase+1, Y+8) == colorCorda) &&
		     (get_pixel(X+desfase+2, Y+8) == 0 || get_pixel(X+desfase+2, Y+8) == colorCorda) &&
		     (get_pixel(X+desfase+3, Y+8) == 0 || get_pixel(X+desfase+3, Y+8) == colorCorda) &&
		     (get_pixel(X+desfase+4, Y+8) == 0 || get_pixel(X+desfase+4, Y+8) == colorCorda) &&
		     (get_pixel(X+desfase+5, Y+8) == 0 || get_pixel(X+desfase+5, Y+8) == colorCorda) &&
		     (get_pixel(X+desfase+6, Y+8) == 0 || get_pixel(X+desfase+6, Y+8) == colorCorda) &&
		     (get_pixel(X+desfase+7, Y+8) == 0 || get_pixel(X+desfase+7, Y+8) == colorCorda) &&
			 (get_pixel(X+desfase  , Y+9) == 0 || get_pixel(X+desfase  , Y+9) == colorCorda) &&
		     (get_pixel(X+desfase+1, Y+9) == 0 || get_pixel(X+desfase+1, Y+9) == colorCorda) &&
		     (get_pixel(X+desfase+2, Y+9) == 0 || get_pixel(X+desfase+2, Y+9) == colorCorda) &&
		     (get_pixel(X+desfase+3, Y+9) == 0 || get_pixel(X+desfase+3, Y+9) == colorCorda) &&
		     (get_pixel(X+desfase+4, Y+9) == 0 || get_pixel(X+desfase+4, Y+9) == colorCorda) &&
		     (get_pixel(X+desfase+5, Y+9) == 0 || get_pixel(X+desfase+5, Y+9) == colorCorda) &&
		     (get_pixel(X+desfase+6, Y+9) == 0 || get_pixel(X+desfase+6, Y+9) == colorCorda) &&
		     (get_pixel(X+desfase+7, Y+9) == 0 || get_pixel(X+desfase+7, Y+9) == colorCorda)
		   );
}

bool ArounderProcesor::baixarEscalo() {
	return ( get_pixel(X  , Y+8) == 0 &&
		     get_pixel(X+1, Y+8) == 0 &&
		     get_pixel(X+2, Y+8) == 0 &&
		     get_pixel(X+3, Y+8) == 0 &&
		     get_pixel(X+4, Y+8) == 0 &&
		     get_pixel(X+5, Y+8) == 0 &&
		     get_pixel(X+6, Y+8) == 0 &&
		     get_pixel(X+7, Y+8) == 0 &&

			 ( get_pixel(X  , Y+9) != 0 ||
			   get_pixel(X+1, Y+9) != 0 ||
		       get_pixel(X+2, Y+9) != 0 ||
		       get_pixel(X+3, Y+9) != 0 ||
		       get_pixel(X+4, Y+9) != 0 ||
		       get_pixel(X+5, Y+9) != 0 ||
		       get_pixel(X+6, Y+9) != 0 ||
		       get_pixel(X+7, Y+9) != 0
			 )
		   );
}

bool ArounderProcesor::blockCaminar() {
	if (O == ORIENT_DRETA) {
		return (get_pixel(X+8, Y+6) != 0 && get_pixel(X+8, Y+7) != 0 && get_pixel(X+8, Y+6) != colorEscalo);
	} else {
		return (get_pixel(X-1, Y+6) != 0 && get_pixel(X-1, Y+7) != 0 && get_pixel(X-1, Y+6) != colorEscalo);
	}
}

bool ArounderProcesor::pujarEscalo() {
	if (O == ORIENT_DRETA) {
		return ( (get_pixel(X+8, Y+6) == 0 || get_pixel(X+8, Y+6) == colorEscalo) && get_pixel(X+8, Y+7) != 0);
	} else {
		return ( (get_pixel(X-1, Y+6) == 0 || get_pixel(X-1, Y+6) == colorEscalo) && get_pixel(X-1, Y+7) != 0);
	}
}

bool ArounderProcesor::blockArounder() {
	bool resultado = false;
	if (anterior != NULL) resultado = anterior->checkArounderAnt(X, Y, O);
	if (siguiente != NULL) resultado = resultado || siguiente->checkArounderSig(X, Y, O);

	return resultado;
}

bool ArounderProcesor::blockParet(int desfase) {
	if (O == ORIENT_DRETA) {
		return (get_pixel(X+8-desfase, Y  ) != 0 ||
				get_pixel(X+8-desfase, Y+1) != 0 ||
				get_pixel(X+8-desfase, Y+2) != 0 ||
				get_pixel(X+8-desfase, Y+3) != 0 ||
				get_pixel(X+8-desfase, Y+4) != 0 ||
				get_pixel(X+8-desfase, Y+5) != 0 ||
				get_pixel(X+8-desfase, Y+6) != 0// ||
				//get_pixel(X+8-desfase, Y+7) != 0
			   );
	} else {
		return (get_pixel(X-1+desfase, Y  ) != 0 ||
				get_pixel(X-1+desfase, Y+1) != 0 ||
				get_pixel(X-1+desfase, Y+2) != 0 ||
				get_pixel(X-1+desfase, Y+3) != 0 ||
				get_pixel(X-1+desfase, Y+4) != 0 ||
				get_pixel(X-1+desfase, Y+5) != 0 ||
				get_pixel(X-1+desfase, Y+6) != 0// ||
				//get_pixel(X-1+desfase, Y+7) != 0
			   );
	}
}

bool ArounderProcesor::blockTecho(int desfase) {
	if ( O == ORIENT_DRETA) desfase = -desfase;

	return ( (get_pixel(X+desfase  , Y-1) != 0 && get_pixel(X+desfase  , Y-1) != colorCorda) ||
		     (get_pixel(X+desfase+1, Y-1) != 0 && get_pixel(X+desfase+1, Y-1) != colorCorda) ||
		     (get_pixel(X+desfase+2, Y-1) != 0 && get_pixel(X+desfase+2, Y-1) != colorCorda) ||
		     (get_pixel(X+desfase+3, Y-1) != 0 && get_pixel(X+desfase+3, Y-1) != colorCorda) ||
		     (get_pixel(X+desfase+4, Y-1) != 0 && get_pixel(X+desfase+4, Y-1) != colorCorda) ||
		     (get_pixel(X+desfase+5, Y-1) != 0 && get_pixel(X+desfase+5, Y-1) != colorCorda) ||
		     (get_pixel(X+desfase+6, Y-1) != 0 && get_pixel(X+desfase+6, Y-1) != colorCorda) ||
		     (get_pixel(X+desfase+7, Y-1) != 0 && get_pixel(X+desfase+7, Y-1) != colorCorda)
		   );
}

bool ArounderProcesor::blockPrecipici() {
	if (O == ORIENT_DRETA) {
		return (get_pixel(X+7, Y+8) == 0 &&
				get_pixel(X+6, Y+8) == 0 &&
				get_pixel(X+5, Y+8) == 0 &&
				get_pixel(X+7, Y+9) == 0 &&
				get_pixel(X+6, Y+9) == 0 &&
				get_pixel(X+5, Y+9) == 0
			   );
	} else {
		return (get_pixel(X  , Y+8) == 0 &&
				get_pixel(X+1, Y+8) == 0 &&
				get_pixel(X+2, Y+8) == 0 &&
				get_pixel(X  , Y+9) == 0 &&
				get_pixel(X+1, Y+9) == 0 &&
				get_pixel(X+2, Y+9) == 0
			   );
	}
}

bool ArounderProcesor::blockPrecipiciPasarela() {
	if (O == ORIENT_DRETA) {
		return ((get_pixel(X+7, Y+8) == 0 || get_pixel(X+7, Y+8) == colorEscalo) &&
				(get_pixel(X+6, Y+8) == 0 || get_pixel(X+6, Y+8) == colorEscalo) &&
				(get_pixel(X+5, Y+8) == 0 || get_pixel(X+5, Y+8) == colorEscalo) &&
				(get_pixel(X+7, Y+9) == 0 || get_pixel(X+7, Y+9) == colorEscalo) &&
				(get_pixel(X+6, Y+9) == 0 || get_pixel(X+6, Y+9) == colorEscalo) &&
				(get_pixel(X+5, Y+9) == 0 || get_pixel(X+5, Y+9) == colorEscalo)
			   );
	} else {
		return ((get_pixel(X  , Y+8) == 0 || get_pixel(X  , Y+8) == colorEscalo) &&
				(get_pixel(X+1, Y+8) == 0 || get_pixel(X+1, Y+8) == colorEscalo) &&
				(get_pixel(X+2, Y+8) == 0 || get_pixel(X+2, Y+8) == colorEscalo) &&
				(get_pixel(X  , Y+9) == 0 || get_pixel(X  , Y+9) == colorEscalo) &&
				(get_pixel(X+1, Y+9) == 0 || get_pixel(X+1, Y+9) == colorEscalo) &&
				(get_pixel(X+2, Y+9) == 0 || get_pixel(X+2, Y+9) == colorEscalo)
			   );
	}
}

bool ArounderProcesor::blockNovaCorda() {
	if (O == ORIENT_DRETA) {
		return (get_pixel(X+6, Y+8+altura) == 0 &&
				get_pixel(X+6, Y+9+altura) == 0 &&
				get_pixel(X+6, Y+10+altura) == 0
			   );
	} else {
		return (get_pixel(X+1, Y+8+altura) == 0 &&
				get_pixel(X+1, Y+9+altura) == 0 &&
				get_pixel(X+1, Y+10+altura) == 0
			   );
	}
}

bool ArounderProcesor::blockCordaBaixar(int desfase) {
	if (O == ORIENT_DRETA) {
		return (get_pixel(X+6-desfase, Y+8) == colorCorda);
	} else {
		return (get_pixel(X+1+desfase, Y+8) == colorCorda);
	}
}

bool ArounderProcesor::blockCordaPujar(int desfase) {
	if (O == ORIENT_DRETA) {
		return (get_pixel(X+6-desfase, Y+5) == colorCorda);
	} else {
		return (get_pixel(X+1+desfase, Y+5) == colorCorda);
	}
}

bool ArounderProcesor::checkArounderAnt(int x, int y, int o) {
	if ( accio == ACCIO_PARAR && (y >= Y-8) && (y <= Y+8) && ( (o == ORIENT_DRETA && X == x+8) || (o == ORIENT_ESQUERRA && X == x-8) ) ) {
		return true;
	} else {
		if (anterior != NULL) {
			return anterior->checkArounderAnt(x, y, o);
		} else {
			return false;
		}
	}
}

bool ArounderProcesor::checkArounderSig(int x, int y, int o) {
	if ( accio == ACCIO_PARAR && (y >= Y-8) && (y <= Y+8) && ( (o == ORIENT_DRETA && X == x+8) || (o == ORIENT_ESQUERRA && X == x-8) ) ) {
		return true;
	} else {
		if (siguiente != NULL) {
			return siguiente->checkArounderSig(x, y, o);
		} else {
			return false;
		}
	}
}

void ArounderProcesor::doCavar() {
	if (O == ORIENT_DRETA) {
		put_pixel(X+8, Y, 0);
		put_pixel(X+8, Y+1, 0);
		put_pixel(X+8, Y+2, 0);
		put_pixel(X+8, Y+3, 0);
		put_pixel(X+8, Y+4, 0);
		put_pixel(X+8, Y+5, 0);
		put_pixel(X+8, Y+6, 0);
		put_pixel(X+8, Y+7, 0);
	} else {
		put_pixel(X-1, Y, 0);
		put_pixel(X-1, Y+1, 0);
		put_pixel(X-1, Y+2, 0);
		put_pixel(X-1, Y+3, 0);
		put_pixel(X-1, Y+4, 0);
		put_pixel(X-1, Y+5, 0);
		put_pixel(X-1, Y+6, 0);
		put_pixel(X-1, Y+7, 0);
	}
}

void ArounderProcesor::doPerforar() {
	put_pixel(X  , Y+8, 0);
	put_pixel(X+1, Y+8, 0);
	put_pixel(X+2, Y+8, 0);
	put_pixel(X+3, Y+8, 0);
	put_pixel(X+4, Y+8, 0);
	put_pixel(X+5, Y+8, 0);
	put_pixel(X+6, Y+8, 0);
	put_pixel(X+7, Y+8, 0);
}

void ArounderProcesor::doEscalera(int desfase) {
	if (O == ORIENT_DRETA) {
		put_pixel(X+5, Y+7+desfase, colorEscalo);
		put_pixel(X+6, Y+7+desfase, colorEscalo);
		put_pixel(X+7, Y+7+desfase, colorEscalo);
	} else {
		put_pixel(X+2, Y+7+desfase, colorEscalo);
		put_pixel(X+1, Y+7+desfase, colorEscalo);
		put_pixel(X  , Y+7+desfase, colorEscalo);
	}
}

void ArounderProcesor::doCorda() {
	if (O == ORIENT_DRETA) {
		put_pixel(X+6, Y+8+altura, colorCorda);
	} else {
		put_pixel(X+1, Y+8+altura, colorCorda);
	}
}

Uint32 ArounderProcesor::get_pixel( int x, int y )
{
    return pixels[ ( y * mapa->w ) + x ];
}

void ArounderProcesor::put_pixel( int x, int y, Uint32 pixel )
{
    pixels[ ( y * mapa->w ) + x ] = pixel;
}
