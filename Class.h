#pragma once
#include	"Variable.h"

const int KEYSIZE = 4;
const int BOXSIZE = 1;
const int POINTSIZE = 1;

class Game {
public:
	Game();
	~Game();
	void Start();

private:
	int playerSpeed;
	int savePlayerSpeed;
	int boxSpeed;

	Vector2 line;

	Circle player;
	Box* box;
	Circle* point;

	void Move();
	void Judge(bool direction, int value);
	void Draw();
};