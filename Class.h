#pragma once
#include	"Variable.h"

struct Vector2 {
	int x;
	int y;
};

struct Circle {
	Vector2 pos;
	int radius;
	int color;
};

struct Player {
	int* score;
	int* hiScore;

	int flagSize;

	Circle shape;

	Vector2 speed;
	Vector2 saveSpeed;
	Vector2 resetPos;

	bool* flag;

	Vector2* posData;
};

struct Point {
	Circle shape;

	bool inputFlag;
};

struct Box {
	int flagSize;

	Vector2 pos;
	Vector2 savePos;
	Vector2 pointerSavePos;
	Vector2 resetPos;

	Vector2 length;

	Vector2 speed;

	int color;

	double* distance;

	bool* flag;

	int* SE;

	bool* SE_Flag;

	int IMG;
};

struct Wall {
	Vector2 pos;
	Vector2 length;

	int color;

	bool flag;

	int IMG;
};

class Game {
public:
	Game();
	~Game();
	bool Start();

private:
	int state;
	int stage;

	int key;
	int timer;

	int* boxSize;
	int* wallSize;

	int wallDivision_X;
	int wallDivision_Y;

	int saveWall;

	Vector2* m_line;

	Player m_player;

	Box* m_box;
	Vector2** m_box_Data;

	Wall* m_wall;
	int** m_wall_DrawData;

	Wall* m_wall_Save;

	Point* m_point;
	Vector2** m_point_Data;

	//DATA
	//配列管理
	enum {
		STAGE_1,STAGE_2,STAGE_3,STAGE_4,STAGE_5
	};

	enum {
		S, PL, B, PO, W
	};

	//ファイル管理
	FILE** m_data;

	const char** m_errorData;

	//IMG
	int* BG_IG;

	const char** m_errorBG_IG;

	enum {
		BG_IG1 = 1
	};

	//BGM
	int* BG_MS;

	bool* BG_MS_Flag;

	const char** m_errorBG_MS;

	enum {
		BG_MS1 = 1,BG_MS2 = 2
	};

	//SE
	const char** m_errorSE;

	enum {
		INSERT
	};

	enum {
		TITLE,GAME,RESULT,Error
	};

	//PLAYER
	enum {
		DRAW, BOXCOLLISION, COURSE, INPUT, WALLCOLLISION
	};

	enum {
		WALK,STEP
	};
	//BOX
	//当たり判定
	enum {
		PLAYERCOLLISION, BOX_BOXCOLLISION,MOVE = WALLCOLLISION
	};

	//距離
	enum {
		WALL,BOX
	};

	void Delete();
	void Draw();
	void Judge(bool direction, int value);
	void Move();
	void New();
	void Reset();

	void FileOpen();

	bool IntErrorCheck(int value);
	bool FileErrorCheck(FILE* value);
};