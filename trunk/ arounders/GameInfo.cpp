#include "GameInfo.h"
#include "const.h"

GameInfo::GameInfo(int pModeGrafic) {
	fase = -1;
	estado = ESTADO_INTRO;
	modeGrafic = pModeGrafic;
}

GameInfo::~GameInfo(void) {
}
