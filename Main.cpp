#include	"DxLib.h"
#include	"math.h"
#include	"Color.h"
#include	"Class.h"
#include	"Variable.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowText("ëqå…î‘");
	SetGraphMode(WIDTH, HEIGHT, 32);
	ChangeWindowMode(TRUE);
	SetBackgroundColor(0, 0, 0);
	if (DxLib_Init() == -1) return -1;	

	Game game;

	while (1) {
		ClearDrawScreen();

		game.Start();

		ScreenFlip();
		WaitTimer(16);
		if (ProcessMessage() == -1) break;
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;
	}

	DxLib_End();		
	return 0;						
}