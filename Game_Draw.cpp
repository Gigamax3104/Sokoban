#include	<DxLib.h>
#include	"Color.h"
#include	"Class.h"
#include	"Variable.h"

void Game::Draw() {
	ClearDrawScreen();

	switch (state) {
	case TITLE:
		timer++;

		DrawExtendGraph(0, 0, WIDTH, HEIGHT, BG_IG[TITLE], FALSE);

		SetFontSize(150);
		DrawString(WIDTH / 2 - 240, 110, "倉庫番", color[BLACK]);
		DrawString(WIDTH / 2 - 250, 100, "倉庫番", color[GREEN]);

		SetFontSize(70);
		if(timer % (60 * 2) <= 60) DrawString(15, HEIGHT - 150, "スペースキーを押してスタート！", color[PURPLE]);
		break;

	case GAME:
		DrawExtendGraph(0, 0, WIDTH, HEIGHT, BG_IG[BG_IG1], FALSE);

		for (int i = 0; i < m_line[stage].x; i++) if (i != 0) DrawLine(0, i * (HEIGHT / m_line[stage].y), WIDTH, i * (HEIGHT / m_line[stage].y), color[WHITE]);
		for (int i = 0; i < m_line[stage].y; i++) if (i != 0) DrawLine(i * (WIDTH / m_line[stage].x), 0, i * (WIDTH / m_line[stage].x), HEIGHT, color[WHITE]);

		for (int i = 0; i < wallSize[stage]; i++) {
			if (m_wall[i].flag) DrawBox(m_wall[i].pos.x - m_wall[i].length.x / 2, m_wall[i].pos.y - m_wall[i].length.y / 2,
				m_wall[i].pos.x + m_wall[i].length.x / 2, m_wall[i].pos.y + m_wall[i].length.y / 2, m_wall[i].color, TRUE);
		}

		for (int i = 0; i < boxSize[stage]; i++) {
			DrawBox(m_box[i].pos.x - m_box[i].length.x / 2, m_box[i].pos.y - m_box[i].length.y / 2,
				m_box[i].pos.x + m_box[i].length.x / 2, m_box[i].pos.y + m_box[i].length.y / 2, m_box[i].color, TRUE);
		}

		for (int i = 0; i < boxSize[stage]; i++) {
			DrawCircle(m_point[i].shape.pos.x, m_point[i].shape.pos.y, m_point[i].shape.radius, m_point[i].shape.color, TRUE);
		}

		DrawCircle(m_player.shape.pos.x, m_player.shape.pos.y, m_player.shape.radius, m_player.shape.color, TRUE);

		SetFontSize(60);
		DrawFormatString(0, HEIGHT - 60, color[BLUE], "STEP:%d", m_player.score[STEP]);
		DrawFormatString(WIDTH - 400, HEIGHT - 60, color[YELLOW], "SCORE:%d", m_player.score[WALK]);
		break;

	case Error:
		Vector2 pos = { 0,0 };

		SetFontSize(80);
		DrawString(pos.x, pos.y, "[NOT FOUND]", color[RED]);

		pos.y += 80;

		SetFontSize(50);
		DrawString(pos.x, pos.y, "FILE", color[WHITE]);

		pos.y += 50;

		SetFontSize(20);
		for (int i = 0; i < W * (STAGE_5 + 1) + 1; i++) {
			if (m_data[i] == nullptr) {
				DrawFormatString(pos.x, pos.y, color[GREEN], "%s", m_errorData[i]);

				pos.y += 20;
			}
		}

		pos.x += 200;
		pos.y = 80;

		SetFontSize(50);
		DrawString(pos.x, pos.y, "IMG", color[WHITE]);

		pos.y += 50;

		SetFontSize(20);
		for (int i = 0; i < BG_IG1 + 1; i++) {
			if (BG_IG[i] == -1) {
				DrawFormatString(pos.x, pos.y, color[GREEN], "%s", m_errorBG_IG[i]);

				pos.y += 20;
			}
		}

		pos.x += 200;
		pos.y = 80;

		SetFontSize(50);
		DrawString(pos.x, pos.y, "SE", color[WHITE]);

		pos.y += 50;

		SetFontSize(20);
		for (int i = 0; i < INSERT + 1; i++) {
			if (m_box[0].SE[i] == -1) {
				DrawFormatString(pos.x, pos.y, color[GREEN], "%s", m_errorSE[i]);

				pos.y += 20;
			}
		}

		pos.x += 200;
		pos.y = 80;

		SetFontSize(50);
		DrawString(pos.x, pos.y, "BGM", color[WHITE]);

		pos.y += 50;

		SetFontSize(25);
		for (int i = 0; i < BG_MS1 + 1; i++) {
			if (BG_MS[i] == -1) {
				DrawFormatString(pos.x, pos.y, color[GREEN], "%s", m_errorBG_MS[i]);

				pos.y += 20;
			}
		}

		break;
	}

	ScreenFlip();
}

//DrawFormatString(0, 0, color[RED], "PLAYER_X:%d", m_player.shape.pos.x);
//DrawFormatString(0, 20, color[RED], "PLAYER_Y:%d", m_player.shape.pos.y);
//DrawFormatString(0, 20 * i, color[RED], "POINT%d番目:%d", i, (int)m_point[i].inputFlag);
//DrawFormatString(0, 20 * i + 40 * i, color[BLUE], "%d番目:POINT_POS_X:%d", i, m_point[i].shape.pos.x);
//DrawFormatString(0, 20 * i + 20 + 40 * i, color[BLUE], "%d番目:POINT_POS_Y:%d", i, m_point[i].shape.pos.y);
//DrawFormatString(0, 0, color[BLUE], "1MAS_X:%d", WIDTH / m_line[stage].x);
//DrawFormatString(0, 20, color[BLUE], "1MAS_Y:%d", HEIGHT / m_line[stage].y);

//DrawFormatString(0, 60, color[RED], "BOX:%d", (int)m_player.flag[BOXCOLLISION]);
//DrawFormatString(0, 80, color[RED], "WALL:%d", (int)m_player.flag[WALLCOLLISION]);
//DrawFormatString(0, 100, color[RED], "COURSE:左右:%d,上下%d", (int)!m_player.flag[COURSE], (int)m_player.flag[COURSE]);
//DrawFormatString(0, 120, color[RED], "PLAYER_INPUT:%d", (int)m_player.flag[INPUT]);

//for (int i = 0; i < boxSize[stage]; i++) {
//	DrawFormatString(170, 20 * i + 60 * i, color[RED], "%d番目:WALLSAVE:%d", i, (int)m_wall_Save[i].flag);
//	DrawFormatString(170, 20 * i + 20 + 60 * i, color[BLUE], "%d番目:SAVE_WALL_X:%d", i, m_wall_Save[i].pos.x);
//	DrawFormatString(170, 20 * i + 40 + 60 * i, color[BLUE], "%d番目:SAVE_WALL_Y:%d", i, m_wall_Save[i].pos.y);
//}
//DrawFormatString(500, 20 * i + 140 * i, color[BLUE], "%d番目:BOX_SAVEPOS_X:%d", i, m_box[i].savePos.x);
//DrawFormatString(500, 20 * i + 20 + 140 * i, color[BLUE], "%d番目:BOX_SAVEPOS_Y:%d", i, m_box[i].savePos.y);
//DrawFormatString(500, 20 * i + 40 + 140 * i, color[RED], "%d番目:BOX_PLAYER:%d", i, (int)m_box[i].flag[PLAYERCOLLISION]);
//DrawFormatString(500, 20 * i + 60 + 140 * i, color[RED], "%d番目:BOX_BOX:%d", i, (int)m_box[i].flag[BOX_BOXCOLLISION]);
//DrawFormatString(500, 20 * i + 80 + 140 * i, color[RED], "%d番目:BOX_MOVE:%d", i, (int)m_box[i].flag[MOVE]);
//DrawFormatString(500, 20 * i + 100 + 140 * i, color[RED], "%d番目:BOX_COURSE:%d", i, (int)m_box[i].flag[COURSE]);
//DrawFormatString(500, 20 * i + 120 + 140 * i, color[RED], "%d番目:BOX_INPUT:%d", i, (int)m_box[i].flag[INPUT]);
//for (int i = 0; i <= STAGE_5; i++) DrawFormatString(0, 20 * i, color[BLUE], "%d", wallSize[i]);

//DrawFormatString(0, 20 * i, color[RED], "%d", (int)m_box[i].SE_Flag[INSERT]);

//if (m_box[i].flag[PLAYERCOLLISION]) {
//	DrawFormatString(0, 0, color[RED], "BOX_POS_X:%d", m_box[i].pos.x);
//	DrawFormatString(0, 20, color[RED], "BOX_DISTANCE:%d", m_box[i].distance[BOX]);
//}

//DrawFormatString(0, 40, color[RED], "BOX2 and 3_DISTANCE_Y:%d", m_box[2].pos.y - m_box[3].pos.y);
