/*
 *  InputManager.cpp
 *  Arounders
 *
 *  Created by Raimon Zamora on 21/02/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "InputManager.h"
#include "const.h"

InputManager::InputManager(GameInfo *pGameInfo) {
	gameInfo = pGameInfo;

	eixir = false;

	mouseX = 0;
	mouseY = 0;

	mouseButtonLeft = false;
	mouseButtonRight = false;
	mouseButtonMiddle = false;

	oldMouseButtonLeft = false;
	oldMouseButtonRight = false;
	oldMouseButtonMiddle = false;

}

InputManager::~InputManager(void) {
	
}

void InputManager::Update() {
	keystates = SDL_GetKeyState( NULL );	

	while ( SDL_PollEvent( &event ) ) {
		if ( event.type == SDL_QUIT ) eixir = true;
		if( event.type == SDL_MOUSEMOTION ) {
			mouseX = event.motion.x;
			mouseY = event.motion.y;
			if (gameInfo->modeGrafic == MODE_ZOOMX2) {
				mouseX = mouseX>>1;
				mouseY = mouseY>>1;
			}
		}
		if( event.type == SDL_MOUSEBUTTONDOWN ) {
			mouseX = event.button.x;
			mouseY = event.button.y;
			if (gameInfo->modeGrafic == MODE_ZOOMX2) {
				mouseX = mouseX>>1;
				mouseY = mouseY>>1;
			}
			if (event.button.button == SDL_BUTTON_LEFT) {
				oldMouseButtonLeft = mouseButtonLeft;
				mouseButtonLeft = true;
			}
			if (event.button.button == SDL_BUTTON_MIDDLE) {
				oldMouseButtonMiddle = mouseButtonMiddle;
				mouseButtonMiddle = true;
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				oldMouseButtonRight = mouseButtonRight;
				mouseButtonRight = true;
			}
		}
		if( event.type == SDL_MOUSEBUTTONUP ) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				oldMouseButtonLeft = mouseButtonLeft;
				mouseButtonLeft = false;
			}
			if (event.button.button == SDL_BUTTON_MIDDLE) {
				oldMouseButtonMiddle = mouseButtonMiddle;
				mouseButtonMiddle = false;
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				oldMouseButtonRight = mouseButtonRight;
				mouseButtonRight = false;
			}
		}
		if( event.type == SDL_KEYDOWN ) {
			cualquierTecla = true;
		} else {
			cualquierTecla = false;
		}

		if( event.type == SDL_KEYUP ) {
			teclaSoltada = true;
		} else {
			teclaSoltada = false;
		}
	}
}

bool InputManager::BotoPulsat(int boto) {
	switch (boto) {
		case SDL_BUTTON_LEFT:
			if (mouseButtonLeft && !oldMouseButtonLeft) {
				oldMouseButtonLeft = true;
				return true;
			} else {
				return false;
			}
			break;
		case SDL_BUTTON_MIDDLE:
			if (mouseButtonMiddle && !oldMouseButtonMiddle) {
				oldMouseButtonMiddle = true;
				return true;
			} else {
				return false;
			}
			break;
		case SDL_BUTTON_RIGHT:
			if (mouseButtonRight && !oldMouseButtonRight) {
				oldMouseButtonRight = true;
				return true;
			} else {
				return false;
			}
			break;
		default:
			return false;
			break;
	}
}

bool InputManager::BotoSoltat(int boto) {
	switch (boto) {
		case SDL_BUTTON_LEFT:
			if (!mouseButtonLeft && oldMouseButtonLeft) {
				oldMouseButtonLeft = false;
				return true;
			} else {
				return false;
			}
			break;
		case SDL_BUTTON_MIDDLE:
			if (!mouseButtonMiddle && oldMouseButtonMiddle) {
				oldMouseButtonMiddle = false;
				return true;
			} else {
				return false;
			}
			break;
		case SDL_BUTTON_RIGHT:
			if (!mouseButtonRight && oldMouseButtonRight) {
				oldMouseButtonRight = false;
				return true;
			} else {
				return false;
			}
			break;
		default:
			return false;
			break;
	}
}

bool InputManager::TeclaPulsada(int tecla) {
	if (keystates[tecla] != 0) {
		return true;
	} else {
		return false;
	}
}

bool InputManager::Pausa() {
	return keystates[SDLK_p];
}

bool InputManager::Menu() {
	return keystates[SDLK_ESCAPE];
}

