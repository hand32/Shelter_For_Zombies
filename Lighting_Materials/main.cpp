#include "Game.h"
#include <time.h>

int main(int arg, char **argc)
{
	srand(time(NULL));
	CGame game(640, 480, 100, 100);
	
	float fBgColor[4] = {0.9f, 1.0f, 0.9f};
	double dLookAt[3][3] = {{22, 22, 22}, {0, 0, 0}, {0, 1, 0}};
	//ortho 22 22 22
	//perspective 22, 30, 22

	game.Create(arg, argc, fBgColor, dLookAt, false);

	return 0;
}