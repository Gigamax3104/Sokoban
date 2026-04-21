#include	<stdio.h>
#include	<DxLib.h>
#include	<math.h>
#include	"Color.h"
#include	"Class.h"
#include	"Variable.h"

bool Game::Start() {
	switch (state) {
	case TITLE:
		if (BG_MS_Flag[TITLE]) {
			PlaySoundMem(BG_MS[TITLE], DX_PLAYTYPE_LOOP);
			BG_MS_Flag[TITLE] = false;
		}

		Draw();

		if (CheckHitKey(KEY_INPUT_SPACE)) {
			timer = 0;

			StopSoundMem(BG_MS[TITLE]);

			state = GAME;
		}

		break;

	case GAME:
		if (BG_MS_Flag[BG_MS1]) {
			ChangeVolumeSoundMem(128, BG_MS[BG_MS1]);
			PlaySoundMem(BG_MS[BG_MS1], DX_PLAYTYPE_LOOP);
			BG_MS_Flag[BG_MS1] = false;
		}

		for (int i = 0; i < (int)pow(boxSize[stage], 2); i++) {
			if (m_point[i % boxSize[stage]].shape.pos.x == m_box[i / boxSize[stage]].pos.x && m_point[i % boxSize[stage]].shape.pos.y == m_box[i / boxSize[stage]].pos.y) {
				for (int j = 0; j < boxSize[stage]; j++) if (m_point[j].inputFlag) m_point[j].inputFlag = false;

				for (int j = 0; j < i; j++)
					if (m_point[j % boxSize[stage]].shape.pos.x == m_box[j / boxSize[stage]].pos.x && m_point[j % boxSize[stage]].shape.pos.y == m_box[j / boxSize[stage]].pos.y)
						m_point[j % boxSize[stage]].inputFlag = true;

				m_point[i % boxSize[stage]].inputFlag = true;

				m_box[i / boxSize[stage]].color = color[LIGHTBLUE];

				i = (i / boxSize[stage] + 1) * boxSize[stage] - 1;

				m_box[i / boxSize[stage]].pointerSavePos.x = m_point[i % boxSize[stage]].shape.pos.x;
				m_box[i / boxSize[stage]].pointerSavePos.y = m_point[i % boxSize[stage]].shape.pos.y;

				if (m_box[i / boxSize[stage]].SE_Flag[INSERT]) {
					StopSoundMem(m_box[i / boxSize[stage]].SE[INSERT]);

					PlaySoundMem(m_box[i / boxSize[stage]].SE[INSERT], DX_PLAYTYPE_BACK);

					m_box[i / boxSize[stage]].SE_Flag[INSERT] = false;
				}
			}
			else {
				for (int j = 0; j < (int)pow(boxSize[stage], 2); j++) {
					if (m_point[j % boxSize[stage]].shape.pos.x == m_box[j / boxSize[stage]].pos.x && m_point[j % boxSize[stage]].shape.pos.y == m_box[j / boxSize[stage]].pos.y)
						break;

					if (j == (int)pow(boxSize[stage], 2) - 1) m_point[i % boxSize[stage]].inputFlag = false;
				}

				for (int j = 0; j < boxSize[stage]; j++) if (m_point[j].inputFlag) m_point[j].inputFlag = true;

				if (m_point[i % boxSize[stage]].inputFlag) continue;

				m_point[i % boxSize[stage]].inputFlag = false;

				if (m_box[i / boxSize[stage]].pointerSavePos.x != NULL
					&& m_point[i % boxSize[stage]].shape.pos.x == m_box[i / boxSize[stage]].pointerSavePos.x
					&& m_point[i % boxSize[stage]].shape.pos.y == m_box[i / boxSize[stage]].pointerSavePos.y) {
					m_box[i / boxSize[stage]].SE_Flag[INSERT] = true;
				}

				m_box[i / boxSize[stage]].color = color[BLUE];
			}
		}

		Move();

		SetDrawScreen(DX_SCREEN_BACK);
		Draw();

		for (int i = 0; i < boxSize[stage]; i++) {
			if (!m_point[i].inputFlag) break;
			else if (i == boxSize[stage] - 1) {
				for(int j = 0; j < boxSize[stage]; j++)
				while (CheckSoundMem(m_box[j].SE[INSERT])) {
					//
				}

				if (stage < STAGE_5) {
					Delete();

					stage++;

					New();
				}
				else state = RESULT;

			}
		}
		break;

	case RESULT:
		if (BG_MS_Flag[BG_MS2]) {
			StopSoundMem(BG_MS[BG_MS1]);
			PlaySoundMem(BG_MS[BG_MS2], DX_PLAYTYPE_BACK);
			BG_MS_Flag[BG_MS2] = false;
		}

		Draw();

		if (CheckHitKey(KEY_INPUT_R)) {
			timer = 0;

			Delete();

			stage = STAGE_1;

			New();

			if (m_player.score[STEP] < m_player.hiScore[STEP]) m_player.hiScore[STEP] = m_player.score[STEP];
			if (m_player.score[WALK] < m_player.hiScore[WALK]) m_player.hiScore[WALK] = m_player.score[WALK];

			for (int i = 0; i < STEP + 1; i++) m_player.score[i] = 0;
			for (int i = 0; i < BG_MS2 + 1; i++) BG_MS_Flag[i] = true;

			state = GAME;
		}
		else if (CheckHitKey(KEY_INPUT_RETURN)) {
			timer = 0;

			Delete();

			stage = STAGE_1;

			New();

			if (m_player.score[STEP] < m_player.hiScore[STEP]) m_player.hiScore[STEP] = m_player.score[STEP];
			if (m_player.score[WALK] > m_player.hiScore[WALK]) m_player.hiScore[WALK] = m_player.score[WALK];

			for (int i = 0; i < STEP + 1; i++) m_player.score[i] = 0;
			for (int i = 0; i < BG_MS2 + 1; i++) BG_MS_Flag[i] = true;

			state = TITLE;
		}
		else if (CheckHitKey(KEY_INPUT_ESCAPE)) return false;

		break;

	case Error:
		Draw();
		break;
	}

	return true;
}

void Game::Move() {
	if(CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_LEFT))Judge(Horizontal, OPPOSITION);

	else if(CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_RIGHT)) Judge(Horizontal, NORMAL);

	else if(CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_UP)) Judge(Vertical, OPPOSITION);

	else if(CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_DOWN)) Judge(Vertical, NORMAL);

	CheckHitKey(KEY_INPUT_R) ? key++ : key = 0;

	if (key == 1) {
		Reset();
	}
}

void Game::Reset() {
	m_player.shape.pos.x = m_player.resetPos.x;
	m_player.shape.pos.y = m_player.resetPos.y;

	m_player.speed.x = m_player.saveSpeed.x;
	m_player.speed.y = m_player.saveSpeed.y;

	m_player.flag[BOXCOLLISION] = false;
	m_player.flag[WALLCOLLISION] = false;
	m_player.flag[COURSE] = false;

	for (int i = 0; i < boxSize[stage]; i++) {
		m_box[i].pos.x = m_box[i].resetPos.x;
		m_box[i].pos.y = m_box[i].resetPos.y;

		m_box[i].flag[PLAYERCOLLISION] = false;
		m_box[i].flag[BOX_BOXCOLLISION] = false;
		m_box[i].flag[COURSE] = false;
		m_box[i].flag[INPUT] = false;

		for (int j = 0; j < INSERT + 1; j++) m_box[i].SE_Flag[j] = true;

		m_wall_Save[i].pos.x = NULL;
		m_wall_Save[i].pos.y = NULL;

		m_wall_Save[i].length.x = NULL;
		m_wall_Save[i].length.y = NULL;

		m_wall_Save[i].flag = false;
	}
}