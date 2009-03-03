#include "AppController.h"
#include "fileManager.h"
#include "const.h"

int main( int argc, char* args[] )
{
	int modeGrafic = MODE_ZOOMX2;
	char *porDefecto = "data.jrf";
	setResourceFile(porDefecto);

	if (argc > 1) {
		for (int i=1;i<argc;i++) {
			if (!strcmp(args[i], "normal")) modeGrafic = MODE_NORMAL;
			if (!strcmp(args[i], "fullscreen")) modeGrafic = MODE_FULLSCREEN;
			if (args[i][4] == '.') setResourceFile(args[i]);
		}
	}
	AppController *appController;
	appController = new AppController(modeGrafic);

	appController->Go();

	delete appController;

	return 0;

}
