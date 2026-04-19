#include	<DxLib.h>
#include	"Color.h"
#include	"Class.h"
#include	"Variable.h"

const char* dataName[] = {
	"DATA/STAGE/STAGEDATA.txt","DATA/PLAYER/PLAYERDATA_1.txt","DATA/PLAYER/PLAYERDATA_2.txt","DATA/PLAYER/PLAYERDATA_3.txt",
	"DATA/PLAYER/PLAYERDATA_4.txt","DATA/PLAYER/PLAYERDATA_5.txt","DATA/BOX/BOXDATA_1.txt","DATA/BOX/BOXDATA_2.txt",
	"DATA/BOX/BOXDATA_3.txt","DATA/BOX/BOXDATA_4.txt","DATA/BOX/BOXDATA_5.txt","DATA/POINT/POINTDATA_1.txt","DATA/POINT/POINTDATA_2.txt",
	"DATA/POINT/POINTDATA_3.txt","DATA/POINT/POINTDATA_4.txt","DATA/POINT/POINTDATA_5.txt","DATA/WALL/WALLDATA_1.txt",
	"DATA/WALL/WALLDATA_2.txt","DATA/WALL/WALLDATA_3.txt","DATA/WALL/WALLDATA_4.txt","DATA/WALL/WALLDATA_5.txt"
};

const char* BG_IGName[] = {
	"IMG/Title.jpg","IMG/BG_IG1.jpg"
};

const char* musicName[] = {
	"SOUND/Title.mp3","SOUND/BG1.mp3"
};

const char* SE_Name[] = {
	"SE/�͂܂艹.mp3"
};

Game::Game() {
	m_data = new FILE * [W * (STAGE_5 + 1) + 1];
	m_errorData = new const char* [W * (STAGE_5 + 1) + 1];

	m_line = new Vector2[STAGE_5 + 1];

	boxSize = new int[STAGE_5 + 1];

	wallSize = new int[STAGE_5 + 1];

	m_player.score = new int[STEP + 1];

	m_player.flagSize = WALLCOLLISION + 1;

	BG_IG = new int[BG_IG1 + 1];
	m_errorBG_IG = new const char* [BG_IG1 + 1];

	BG_MS = new int[BG_MS1 + 1];
	BG_MS_Flag = new bool[BG_MS1 + 1];
	m_errorBG_MS = new const char*[BG_MS1 + 1];

	m_errorSE = new const char* [BG_MS1 + 1];

	state = TITLE;
	stage = STAGE_2;

	key = 0;
	timer = 0;

	saveWall = 0;

	m_player.flag = new bool[m_player.flagSize];

	m_player.posData = new Vector2[STAGE_5 + 1];

	m_player.speed.x = WIDTH / (60 * 5);
	m_player.speed.y = HEIGHT / (60 * 5);
	m_player.saveSpeed.x = m_player.speed.x;
	m_player.saveSpeed.y = m_player.speed.y;

	m_player.shape.color = color[RED];

	m_player.flag[BOXCOLLISION] = false;
	m_player.flag[WALLCOLLISION] = false;
	m_player.flag[COURSE] = false;

	m_player.score[WALK] = 0;
	m_player.score[STEP] = 0;

	FileOpen();
}

void Game::FileOpen() {
	for (int i = 0; i < W * (STAGE_5 + 1) + 1; i++) {
		fopen_s(&m_data[i], dataName[i], "r");

		if (FileErrorCheck(m_data[i])) state = Error;

		m_errorData[i] = dataName[i];
	}

	for (int i = 0; i < STAGE_5 + 1; i++) {
		fscanf_s(m_data[S], "%d", &m_line[i].x);
		fscanf_s(m_data[S], "%d", &m_line[i].y);

		wallSize[i] = m_line[i].x * m_line[i].y;
	}

	for (int i = 0; i < STAGE_5 + 1; i++) fscanf_s(m_data[(B - 1) * (STAGE_5 + 1) + 1 + i], "%d", &boxSize[i]);

	m_box = new Box[boxSize[stage]];

	m_point = new Point[boxSize[stage]];

	m_wall = new Wall[wallSize[stage]];

	m_wall_Save = new Wall[boxSize[stage]];

	for (int i = 1; i < 1 + STAGE_5 + 1; i++) {
		fscanf_s(m_data[i], "%d", &m_player.posData[i - 1].x);
		fscanf_s(m_data[i], "%d", &m_player.posData[i - 1].y);
	}

	m_player.shape.pos.x = m_player.posData[stage].x * WIDTH / m_line[stage].x - WIDTH / m_line[stage].x / 2;
	m_player.shape.pos.y = m_player.posData[stage].y * HEIGHT / m_line[stage].y - HEIGHT / m_line[stage].y / 2;

	m_player.resetPos.x = m_player.shape.pos.x;
	m_player.resetPos.y = m_player.shape.pos.y;

	m_player.shape.radius = HEIGHT / m_line[stage].y / 4;

	for (int i = 0; i < boxSize[stage]; i++) {
		m_box[i].SE = new int[INSERT + 1];
		m_box[i].SE_Flag = new bool[INSERT + 1];

		fscanf_s(m_data[(B - 1) * (STAGE_5 + 1) + 1 + stage], "%d", &m_box[i].posData.x);
		fscanf_s(m_data[(B - 1) * (STAGE_5 + 1) + 1 + stage], "%d", &m_box[i].posData.y);

		m_box[i].pos.x = m_box[i].posData.x * WIDTH / m_line[stage].x - WIDTH / m_line[stage].x / 2;
		m_box[i].pos.y = m_box[i].posData.y * HEIGHT / m_line[stage].y - HEIGHT / m_line[stage].y / 2;

		m_box[i].savePos.x = m_box[i].pos.x;
		m_box[i].savePos.y = m_box[i].pos.y;

		m_box[i].pointerSavePos.x = NULL;
		m_box[i].pointerSavePos.y = NULL;

		m_box[i].resetPos.x = m_box[i].pos.x;
		m_box[i].resetPos.y = m_box[i].pos.y;

		m_box[i].distance = new double[BOX + 1];

		m_box[i].flagSize = MOVE + 1;
		m_box[i].flag = new bool[m_box[i].flagSize];

		m_box[i].length.x = WIDTH / m_line[stage].x;
		m_box[i].length.y = HEIGHT / m_line[stage].y;

		m_box[i].speed.x = WIDTH / (60 * 6);
		m_box[i].speed.y = HEIGHT / (60 * 6);

		m_box[i].color = color[BLUE];

		m_box[i].distance[BOX] = NULL;

		m_box[i].flag[PLAYERCOLLISION] = false;
		m_box[i].flag[BOX_BOXCOLLISION] = false;
		m_box[i].flag[COURSE] = false;
		m_box[i].flag[INPUT] = false;
		m_box[i].flag[MOVE] = true;

		for (int j = 0; j < INSERT + 1; j++) {
			m_box[i].SE[j] = LoadSoundMem(SE_Name[j]);

			if (IntErrorCheck(m_box[i].SE[j])) state = Error;

			m_box[i].SE_Flag[j] = true;
		}
	}

	for (int i = 0; i < wallSize[stage]; i++) {
		fscanf_s(m_data[(W - 1) * (STAGE_5 + 1) + 1 + stage], "%d", &m_wall[i].drawData);

		m_wall[i].pos.x = WIDTH / m_line[stage].x * (i % m_line[stage].x) + WIDTH / m_line[stage].x / 2;
		m_wall[i].pos.y = HEIGHT / m_line[stage].y * (i / m_line[stage].x) + HEIGHT / m_line[stage].y / 2;

		m_wall[i].length.x = WIDTH / m_line[stage].x + WIDTH % m_line[stage].x;
		m_wall[i].length.y = HEIGHT / m_line[stage].y + HEIGHT % m_line[stage].y;

		m_wall[i].color = color[LIGHTGRAY];

		if (m_wall[i].drawData) m_wall[i].flag = true;
		else m_wall[i].flag = false;
	}

	for (int i = 0; i < boxSize[stage]; i++) {
		m_wall_Save[i].pos.x = NULL;
		m_wall_Save[i].pos.y = NULL;

		m_wall_Save[i].length.x = NULL;
		m_wall_Save[i].length.y = NULL;

		m_wall_Save[i].flag = false;
	}

	for (int i = 0; i < boxSize[stage]; i++) {
		fscanf_s(m_data[(PO - 1) * (STAGE_5 + 1) + 1 + stage], "%d", &m_point[i].posData.x);
		fscanf_s(m_data[(PO - 1) * (STAGE_5 + 1) + 1 + stage], "%d", &m_point[i].posData.y);

		m_point[i].shape.pos.x = m_point[i].posData.x * WIDTH / m_line[stage].x - WIDTH / m_line[stage].x / 2;
		m_point[i].shape.pos.y = m_point[i].posData.y * HEIGHT / m_line[stage].y - HEIGHT / m_line[stage].y / 2;

		m_point[i].shape.radius = HEIGHT / m_line[stage].y / 8;

		m_point[i].shape.color = color[GREEN];

		m_point[i].inputFlag = false;
	}

	for (int i = 0; i < BG_IG1 + 1; i++) {
		BG_IG[i] = LoadGraph(BG_IGName[i]);

		if (IntErrorCheck(BG_IG[i])) state = Error;

		m_errorBG_IG[i] = BG_IGName[i];
	}

	for (int i = 0; i < INSERT + 1; i++) m_errorSE[i] = SE_Name[i];

	for (int i = 0; i < BG_MS1 + 1; i++) {
		BG_MS[i] = LoadSoundMem(musicName[i]);

		if (IntErrorCheck(BG_MS[i])) state = Error;

		BG_MS_Flag[i] = true;

		m_errorBG_MS[i] = musicName[i];
	}

	for (int i = 0; i < W + 1; i++) fclose(m_data[i]);
}

void Game::Delete() {
	delete[] m_box;

	delete[] m_point;

	delete[] m_wall;

	delete[] m_wall_Save;
}

void Game::New() {
	m_box = new Box[boxSize[stage]];

	m_point = new Point[boxSize[stage]];

	m_wall = new Wall[wallSize[stage]];

	m_wall_Save = new Wall[boxSize[stage]];

	m_player.shape.pos.x = m_player.posData[stage].x * WIDTH / m_line[stage].x - WIDTH / m_line[stage].x / 2;
	m_player.shape.pos.y = m_player.posData[stage].y * HEIGHT / m_line[stage].y - HEIGHT / m_line[stage].y / 2;

	m_player.resetPos.x = m_player.shape.pos.x;
	m_player.resetPos.y = m_player.shape.pos.y;

	m_player.shape.radius = HEIGHT / m_line[stage].y / 4;

	for (int i = 0; i < boxSize[stage]; i++) {
		m_box[i].SE = new int[INSERT + 1];
		m_box[i].SE_Flag = new bool[INSERT + 1];

		fscanf_s(m_data[(B - 1) * (STAGE_5 + 1) + 1 + stage], "%d", &m_box[i].posData.x);
		fscanf_s(m_data[(B - 1) * (STAGE_5 + 1) + 1 + stage], "%d", &m_box[i].posData.y);

		m_box[i].pos.x = m_box[i].posData.x * WIDTH / m_line[stage].x - WIDTH / m_line[stage].x / 2;
		m_box[i].pos.y = m_box[i].posData.y * HEIGHT / m_line[stage].y - HEIGHT / m_line[stage].y / 2;

		m_box[i].savePos.x = m_box[i].pos.x;
		m_box[i].savePos.y = m_box[i].pos.y;

		m_box[i].pointerSavePos.x = NULL;
		m_box[i].pointerSavePos.y = NULL;

		m_box[i].resetPos.x = m_box[i].pos.x;
		m_box[i].resetPos.y = m_box[i].pos.y;

		m_box[i].distance = new double[BOX + 1];

		m_box[i].flagSize = MOVE + 1;
		m_box[i].flag = new bool[m_box[i].flagSize];

		m_box[i].length.x = WIDTH / m_line[stage].x;
		m_box[i].length.y = HEIGHT / m_line[stage].y;

		m_box[i].speed.x = WIDTH / (60 * 6);
		m_box[i].speed.y = HEIGHT / (60 * 6);

		m_box[i].color = color[BLUE];

		m_box[i].distance[BOX] = NULL;

		m_box[i].flag[PLAYERCOLLISION] = false;
		m_box[i].flag[BOX_BOXCOLLISION] = false;
		m_box[i].flag[COURSE] = false;
		m_box[i].flag[INPUT] = false;
		m_box[i].flag[MOVE] = true;

		for (int j = 0; j < INSERT + 1; j++) {
			m_box[i].SE[j] = LoadSoundMem(SE_Name[j]);

			if (IntErrorCheck(m_box[i].SE[j])) state = Error;

			m_box[i].SE_Flag[j] = true;
		}
	}

	for (int i = 0; i < wallSize[stage]; i++) {
		fscanf_s(m_data[(W - 1) * (STAGE_5 + 1) + 1 + stage], "%d", &m_wall[i].drawData);

		m_wall[i].pos.x = WIDTH / m_line[stage].x * (i % m_line[stage].x) + WIDTH / m_line[stage].x / 2;
		m_wall[i].pos.y = HEIGHT / m_line[stage].y * (i / m_line[stage].x) + HEIGHT / m_line[stage].y / 2;

		m_wall[i].length.x = WIDTH / m_line[stage].x + WIDTH % m_line[stage].x;
		m_wall[i].length.y = HEIGHT / m_line[stage].y + HEIGHT % m_line[stage].y;

		m_wall[i].color = color[LIGHTGRAY];

		if (m_wall[i].drawData) m_wall[i].flag = true;
		else m_wall[i].flag = false;
	}

	for (int i = 0; i < boxSize[stage]; i++) {
		m_wall_Save[i].pos.x = NULL;
		m_wall_Save[i].pos.y = NULL;

		m_wall_Save[i].length.x = NULL;
		m_wall_Save[i].length.y = NULL;

		m_wall_Save[i].flag = false;
	}

	for (int i = 0; i < boxSize[stage]; i++) {
		fscanf_s(m_data[(PO - 1) * (STAGE_5 + 1) + 1 + stage], "%d", &m_point[i].posData.x);
		fscanf_s(m_data[(PO - 1) * (STAGE_5 + 1) + 1 + stage], "%d", &m_point[i].posData.y);

		m_point[i].shape.pos.x = m_point[i].posData.x * WIDTH / m_line[stage].x - WIDTH / m_line[stage].x / 2;
		m_point[i].shape.pos.y = m_point[i].posData.y * HEIGHT / m_line[stage].y - HEIGHT / m_line[stage].y / 2;

		m_point[i].shape.radius = HEIGHT / m_line[stage].y / 8;

		m_point[i].shape.color = color[GREEN];

		m_point[i].inputFlag = false;
	}

	for (int i = 0; i < W + 1; i++) fclose(m_data[i]);
}

Game::~Game() {
	delete[] m_data;
	m_data = nullptr;

	delete[] m_errorData;
	m_errorData = nullptr;

	delete[] m_line;
	m_line = nullptr;

	delete[] m_player.score;
	m_player.score = nullptr;

	delete[] m_player.flag;
	m_player.flag = nullptr;

	delete[] m_player.posData;
	m_player.posData = nullptr;

	delete[] m_point;
	m_point = nullptr;

	for (int i = 0; i < boxSize[stage] * (INSERT + 1); i++) DeleteSoundMem(m_box[i / boxSize[stage]].SE[i % boxSize[stage]]);

	for (int i = 0; i < boxSize[stage]; i++) {
		delete[] m_box[i].distance;
		m_box[i].distance = nullptr;

		delete[] m_box[i].flag;
		m_box[i].flag = nullptr;

		delete[] m_box[i].SE;
		m_box[i].SE = nullptr;

		delete[] m_box[i].SE_Flag;
		m_box[i].SE_Flag = nullptr;
	}

	delete[] boxSize;

	delete[] m_box;
	m_box = nullptr;

	delete[] wallSize;

	delete[] m_wall;
	m_wall = nullptr;

	delete[] m_wall_Save;
	m_wall_Save = nullptr;

	for (int i = 0; i < BG_MS1 + 1; i++) DeleteSoundMem(BG_MS[i]);

	delete[] BG_MS;
	BG_MS = nullptr;

	delete[] BG_MS_Flag;
	BG_MS_Flag = nullptr;

	delete[] m_errorBG_MS;
	m_errorBG_MS = nullptr;

	delete[] m_errorSE;
	m_errorSE = nullptr;
}
