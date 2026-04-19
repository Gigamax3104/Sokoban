#include	<DxLib.h>
#include	"Class.h"

bool Game::IntErrorCheck(int value) {
	return value == -1 || value == NULL ? true : false;
}

bool Game::FileErrorCheck(FILE* value) {
	return value == nullptr ? true : false;
}