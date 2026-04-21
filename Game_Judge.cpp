#include	<DxLib.h>
#include	<math.h>
#include	"Class.h"
#include	"Color.h"
#include	"Variable.h"

void Game::Judge(bool direction, int value) {
	bool wallSaveFlag = true;

	//左右を押された時
	if (direction) {
		//プレイヤーのx座標の保存
		int save = m_player.shape.pos.x;

		//画面制限
		if ((value == OPPOSITION && save + (WIDTH / m_line[stage].x) * value > 0)
			|| value == NORMAL && save + (WIDTH / m_line[stage].x) * value < WIDTH) {

			//箱接続フラグ(不成立)
			if (!m_player.flag[BOXCOLLISION]) {
				//マスからマスへの移動
				while ((value == OPPOSITION && m_player.shape.pos.x > save + (WIDTH / m_line[stage].x) * value)
					|| value == NORMAL && m_player.shape.pos.x < save + (WIDTH / m_line[stage].x) * value) {

					//プレイヤーと壁との距離判定(接続していたら壁接続を成立させる)。
					for (int i = 0; i < wallSize[stage]; i++) {
						if (m_wall[i].flag)
							if (pow(save + (WIDTH / m_line[stage].x) * value - m_wall[i].pos.x, 2) <= pow(m_player.shape.radius + m_wall[i].length.x / 2, 2)
								&& pow(m_player.shape.pos.y - m_wall[i].pos.y, 2) <= pow(m_player.shape.radius + m_wall[i].length.y / 2, 2))
								m_player.flag[WALLCOLLISION] = true;
					}

					//プレイヤーと箱との調べ
					for (int i = 0; i < boxSize[stage]; i++) {
						//x座標の移動先に箱と接続していたら描画フラグを不成立させる(そうでなければフラグを成立させる)。
						if (pow(save + (WIDTH / m_line[stage].x) * value - m_box[i].pos.x, 2) <= pow(m_player.shape.radius + m_box[i].length.x / 2, 2)
							&& pow(m_player.shape.pos.y - m_box[i].pos.y, 2) <= pow(m_player.shape.radius + m_box[i].length.y / 2, 2))
							m_player.flag[DRAW] = false;

						//プレイヤーと箱との位置を調べ、接続していたら以下の処理を行う。
						if (pow(m_player.shape.pos.x - m_box[i].pos.x, 2) <= pow(m_player.shape.radius + m_box[i].length.x / 2, 2)
							&& pow(m_player.shape.pos.y - m_box[i].pos.y, 2) <= pow(m_player.shape.radius + m_box[i].length.y / 2, 2)) {

							//最初に保存しておいた場所に移動する。
							m_player.shape.pos.x = save;

							//プレイヤーの移動の速さを箱の速さと同じにする。
							m_player.speed.x = m_box[i].speed.x;
							m_player.speed.y = m_box[i].speed.y;

							//箱接続フラグを成立させる。
							m_player.flag[BOXCOLLISION] = true;

							//コース(上下)フラグを不成立させる。
							m_player.flag[COURSE] = false;

							//Aキー:入力フラグ不成立、Dキー:入力フラグ成立。
							value == OPPOSITION ? m_player.flag[INPUT] = false :
								m_player.flag[INPUT] = true;

							//今の箱のx座標の位置を保存する。
							m_box[i].savePos.x = m_box[i].pos.x;

							//箱のプレイヤー接続フラグを成立させる。
							m_box[i].flag[PLAYERCOLLISION] = true;
						}
					}

					//箱と壁の接続フラグが不成立の場合のみ処理
					if (!m_player.flag[BOXCOLLISION] && !m_player.flag[WALLCOLLISION]) {

						//プレイヤーの移動の速さを保存しておいた速さへ再構築
						m_player.speed.x = m_player.saveSpeed.x;
						m_player.speed.y = m_player.saveSpeed.y;

						//プレイヤーのx座標の移動
						m_player.shape.pos.x += m_player.speed.x * value;

						//移動先より先に行ってしまった場合に戻す
						if ((value == OPPOSITION && m_player.shape.pos.x <= save + (WIDTH / m_line[stage].x) * value)
							|| (value == NORMAL && m_player.shape.pos.x >= save + (WIDTH / m_line[stage].x) * value)) {
							m_player.score[STEP]++;
							m_player.score[WALK] += 10;

							m_player.shape.pos.x = save + (WIDTH / m_line[stage].x) * value;
						}
					}

					//描画フラグ成立の際、描画する。
					if (m_player.flag[DRAW]) {
						SetDrawScreen(DX_SCREEN_BACK);
						Draw();
					}

					//箱と壁の接続フラグ成立時、すぐさまに移動をやめる(同時に、壁の接続フラグを不成立・描画フラグを成立させる)
					if (m_player.flag[BOXCOLLISION] || m_player.flag[WALLCOLLISION]) {
						m_player.flag[WALLCOLLISION] = false;

						m_player.flag[DRAW] = true;
						break;
					}
				}

			}
			//箱接続フラグ(成立)
			else {
				//マスからマスへの移動
				while ((value == OPPOSITION && m_player.shape.pos.x > save + (WIDTH / m_line[stage].x) * value)
					|| value == NORMAL && m_player.shape.pos.x < save + (WIDTH / m_line[stage].x) * value) {

					for (int i = 0; i < boxSize[stage]; i++) {
						//箱のプレイヤー接続フラグが成立の時、箱の移動方向を左右(false)にする。
						if(m_box[i].flag[PLAYERCOLLISION]) m_box[i].flag[COURSE] = false;

						//入力が接触する前と接触した後で逆方向だった場合か、上下に移動した時以下の処理を行う。
						if ((m_player.flag[INPUT] && m_player.shape.pos.x + m_player.speed.x * value < save)
							|| !m_player.flag[INPUT] && m_player.shape.pos.x + m_player.speed.x * value > save
							|| m_player.flag[COURSE]) {

							//箱接続フラグを不成立させる。
							m_player.flag[BOXCOLLISION] = false;

							for (int j = 0; j < boxSize[stage]; j++) {

								//もしプレイヤー接続フラグが成立の場合は、不成立にさせる。
								if (m_box[j].flag[PLAYERCOLLISION]) {
									m_box[j].flag[PLAYERCOLLISION] = false;
									m_box[i].flag[MOVE] = true;
								}
							}

							//プレイヤーの動きを元の速さにする。
							m_player.speed.x = m_player.saveSpeed.x;
							m_player.speed.y = m_player.saveSpeed.y;
						}

						if (m_box[i].flag[PLAYERCOLLISION] &&
							(value == OPPOSITION && m_box[i].pos.x - m_box[i].length.x / 2 <= 0
							|| value == NORMAL && m_box[i].pos.x + m_box[i].length.x / 2 >= WIDTH)) {
							return;
						}

						//プレイヤー接続フラグと、壁保存フラグが成立している時、以下の処理を行う。(※1)
						if (m_box[i].flag[PLAYERCOLLISION] && m_wall_Save[i].flag) {

							//箱が移動した時に、接続している壁の位置と接続した時、以下の処理を行う。
							if ((value == NORMAL && pow(m_box[i].savePos.x + WIDTH / m_line[stage].x * value + m_player.speed.x - m_wall_Save[i].pos.x, 2) <= pow(m_box[i].length.x / 2 + m_wall_Save[i].length.x / 2, 2)
								&& pow(m_box[i].pos.y - m_wall_Save[i].pos.y, 2) <= pow(m_box[i].length.y / 2 + m_wall_Save[i].length.y / 2, 2))
								|| value == OPPOSITION && pow(m_box[i].savePos.x - WIDTH / m_line[stage].x * value - m_player.speed.x - m_wall_Save[i].pos.x, 2) >= pow(m_box[i].length.x / 2 + m_wall_Save[i].length.x / 2, 2)
								&& pow(m_box[i].pos.y - m_wall_Save[i].pos.y, 2) <= pow(m_box[i].length.y / 2 + m_wall_Save[i].length.y / 2, 2)) {
								
								//プレイヤーの壁接続フラグを成立させる。
								m_player.flag[WALLCOLLISION] = true;

								//箱が移動した時に、接続している壁のx座標が同じか違う時かつ壁のy座標が違う時、以下の処理を行う。
								if (m_box[i].savePos.x + WIDTH / m_line[stage].x * value + m_player.speed.x != m_wall_Save[i].pos.x && m_box[i].pos.y != m_wall_Save[i].pos.y
									|| m_box[i].savePos.x + WIDTH / m_line[stage].x * value + m_player.speed.x == m_wall_Save[i].pos.x && m_box[i].pos.y != m_wall_Save[i].pos.y) {
									
									//プレイヤーの壁接続フラグを不成立にさせる。
									m_player.flag[WALLCOLLISION] = false;

									m_box[i].flag[MOVE] = true;

									//壁保存フラグを不成立にさせる。
									m_wall_Save[i].flag = false;

									//箱の保存位置のx座標を移動後に代入する。
									m_box[i].savePos.x = m_box[i].savePos.x + WIDTH / m_line[stage].x * value;

									//関数内壁保存フラグを不成立にさせる。
									wallSaveFlag = false;
								}

							}
							//※1でない時、以下の処理を行う。
							else {
								//プレイヤーの壁接続フラグを不成立にさせる。
								m_player.flag[WALLCOLLISION] = false;
								m_box[i].flag[MOVE] = true;

								for (int j = 0; j < boxSize[stage] * wallSize[stage]; j++) {

									//壁と壁保存の位置が違う時かつ、箱のプレイヤー接続フラグと壁の描画フラグが成立の時かつ、
									//箱と壁が接触していた時、以下の処理を行う。
									if (m_wall[j % wallSize[stage]].pos.x != m_wall_Save[j / wallSize[stage]].pos.x && m_wall[j % wallSize[stage]].pos.y != m_wall_Save[j / wallSize[stage]].pos.y) {
										if (m_box[j / wallSize[stage]].flag[PLAYERCOLLISION] && m_wall[j % wallSize[stage]].flag) {
											if (pow(m_box[j / wallSize[stage]].pos.x - m_wall[j % wallSize[stage]].pos.x, 2) <= pow(m_box[j / wallSize[stage]].length.x / 2 + m_wall[j % wallSize[stage]].length.x / 2, 2)
												&& pow(m_box[j / wallSize[stage]].pos.y - m_wall[j % wallSize[stage]].pos.y, 2) <= pow(m_box[j / wallSize[stage]].length.y / 2 + m_wall[j % wallSize[stage]].length.y / 2, 2)) {
												
												//プレイヤーの壁接続フラグを成立させる。
												m_player.flag[WALLCOLLISION] = true;

												//箱の移動後に壁の位置と同じだった時、以下の処理を行う。
												for (int k = 0; k < wallSize[stage]; k++) {
													if (m_box[j / wallSize[stage]].savePos.x + WIDTH / m_line[stage].x * value == m_wall[k].pos.x
														&& m_box[j / wallSize[stage]].pos.y == m_wall[k].pos.y && !m_wall[k].flag) {

														//プレイヤーの壁接続フラグを不成立にさせる。
														m_player.flag[WALLCOLLISION] = false;
														m_box[i].flag[MOVE] = true;
													}
												}
											}
										}
									}
								}

								//プレイヤーの壁接続フラグが不成立の時、壁保存フラグを不成立にさせ、箱の保存位置のx座標を移動後に代入にし、
								//関数内壁保存フラグを不成立にさせる。
								if (!m_player.flag[WALLCOLLISION]) {
									m_wall_Save[i].flag = false;

									if(m_box[i].pos.x == m_box[i].savePos.x + WIDTH / m_line[stage].x * value)
									m_box[i].savePos.x = m_box[i].savePos.x + WIDTH / m_line[stage].x * value;

									wallSaveFlag = false;
								}
								else {

								}
							}
						}

						//箱のプレイヤー接続フラグが成立の時、以下の処理を行う。
						if (m_box[i].flag[PLAYERCOLLISION]) {

							//箱の入力フラグを右の時は成立、左の時は不成立にさせる。
							if (value == NORMAL) m_box[i].flag[INPUT] = true;
							else m_box[i].flag[INPUT] = false;

							for (int j = 0; j < boxSize[stage]; j++) {

								//調べている箱と違う箱の時かつ、箱との距離が保存している距離より小さい時かつ、
								// 箱の箱接続フラグが不成立の時、以下の処理を行う。
								if (i != j && m_box[i].distance[BOX] <= pow(m_box[i].pos.x - m_box[j].pos.x, 2) + pow(m_box[i].pos.y - m_box[j].pos.y, 2)
									&& !m_box[i].flag[BOX_BOXCOLLISION]) {

									//箱と箱が接触している時、以下の処理を行う。
									if (pow(m_box[i].pos.x - m_box[j].pos.x, 2) <= pow(m_box[i].length.x / 2 + m_box[j].length.x / 2, 2)
										&& pow(m_box[i].pos.y - m_box[j].pos.y, 2) <= pow(m_box[i].length.y / 2 + m_box[j].length.y / 2, 2)) {
										
										//箱の箱接続フラグを成立させる。
										m_box[i].flag[BOX_BOXCOLLISION] = true;


										//箱が上下に動いた時、または左右で逆方向に移動する時、または箱と箱の位置のx座標が同じか違う時かつ、
										//y座標が違う時以下の処理を行う。(※2)
										if (m_box[i].flag[COURSE]
											|| value == NORMAL && !m_box[i].flag[INPUT] || value == OPPOSITION && m_box[i].flag[INPUT]
												|| m_box[i].pos.x - m_box[j].pos.x != 0 && m_box[i].pos.y - m_box[j].pos.y != 0
													|| m_box[i].pos.x - m_box[j].pos.x == 0 && m_box[i].pos.y - m_box[j].pos.y != 0) {

											//箱の箱接続フラグを不成立にさせる。
											m_box[i].flag[BOX_BOXCOLLISION] = false;
											m_box[i].flag[MOVE] = true;

										}

										//保存距離を箱との距離に代入する。
										m_box[i].distance[BOX]
											= pow(m_box[i].pos.x - m_box[j].pos.x, 2) + pow(m_box[i].pos.y - m_box[j].pos.y, 2);
									}
									//※2でない時、箱の箱接続フラグを不成立にさせる。。
									else {
										m_box[i].flag[BOX_BOXCOLLISION] = false;
										m_box[i].flag[MOVE] = true;
									}
								}
								else if(m_box[i].savePos.x + WIDTH / m_line[stage].x * value == m_box[j].pos.x
									&& m_box[i].pos.y == m_box[j].pos.y) {
									m_box[i].flag[BOX_BOXCOLLISION] = true;
								}
							}
						}

						for (int i = 0; i < boxSize[stage]; i++){
							
							//箱の箱接続フラグが成立の時、以下の処理を行う。
							if (m_box[i].flag[BOX_BOXCOLLISION]) {

								//動作フラグと箱の箱接続フラグと箱の移動方向を左右にさせ、入力フラグを右の時は成立、
								// 左の時は不成立にさせ、保存距離をなくし、関数を抜ける。
								m_box[i].flag[MOVE] = true;
								m_box[i].flag[BOX_BOXCOLLISION] = false;

								m_box[i].flag[COURSE] = false;

								if (value == NORMAL) m_box[i].flag[INPUT] = true;
								else m_box[i].flag[INPUT] = false;

								m_box[i].distance[BOX] = NULL;

								return;
							}
						}

						//プレイヤー接続フラグが成立かつ、箱の箱接続フラグとプレイヤーの壁接続フラグが不成立の時、以下の処理を行う。
						if (m_box[i].flag[PLAYERCOLLISION] && !m_box[i].flag[BOX_BOXCOLLISION] && !m_player.flag[WALLCOLLISION]) {

							//箱接続フラグが成立の時、以下の処理を行う。
							if (m_player.flag[BOXCOLLISION]) {
								//プレイヤーのx座標を入力に対応したプレイヤーの移動距離分進める。
								m_player.shape.pos.x += m_player.speed.x * value;

								//移動方向が左右の時、以下の処理を行う。
								if (!m_player.flag[COURSE]) {

									//箱のx座標を入力に対応した箱の移動距離分進める。
									m_box[i].pos.x += m_box[i].speed.x * value;

									//プレイヤーの位置が移動先より奥か丁度の時、以下の処理を行う。
									if ((value == OPPOSITION && m_player.shape.pos.x <= save + (WIDTH / m_line[stage].x) * value)
										|| value == NORMAL && m_player.shape.pos.x >= save + (WIDTH / m_line[stage].x) * value) {
										
										//プレイヤーの位置を移動先へ戻す。
										m_player.shape.pos.x = save + (WIDTH / m_line[stage].x) * value;

										m_player.score[STEP]++;
										m_player.score[WALK] += 10;

										//関数内壁保存フラグが成立の時、箱のx座標を移動先に戻し、x座標の保存位置を今の位置に代入する。
										if (wallSaveFlag) {
											m_box[i].pos.x = m_box[i].savePos.x + (WIDTH / m_line[stage].x) * value;
											m_box[i].savePos.x = m_box[i].pos.x;
										}

										//箱の入力フラグを右の時は成立、左の時は不成立にさせる。
										if (value == NORMAL) m_box[i].flag[INPUT] = true;
										else m_box[i].flag[INPUT] = false;

										//保存距離をなくす。
										m_box[i].distance[BOX] = NULL;
									}
								}
							}
						}
						m_box[i].flag[MOVE] = true;
					}

					//プレイヤーの箱接続フラグが不成立の時、またはプレイヤーの壁接続フラグが成立の時、抜ける。
					if (!m_player.flag[BOXCOLLISION] || m_player.flag[WALLCOLLISION]) break;

					//描画
					SetDrawScreen(DX_SCREEN_BACK);
					Draw();
				}
			}
		}
	}
	//上下を押された時(以降ほぼほぼ一緒のため、省略あり)
	else {
		int save = m_player.shape.pos.y;

		if ((value == OPPOSITION && save + HEIGHT / m_line[stage].y * value > 0)
			|| value == NORMAL && save + HEIGHT / m_line[stage].y * value < HEIGHT) {

			if (!m_player.flag[BOXCOLLISION]) {
				while ((value == OPPOSITION && m_player.shape.pos.y > save + HEIGHT / m_line[stage].y * value)
					|| value == NORMAL && m_player.shape.pos.y < save + HEIGHT / m_line[stage].y * value) {

					for (int i = 0; i < wallSize[stage]; i++) {
						if (m_wall[i].flag)
							if (pow(m_player.shape.pos.x - m_wall[i].pos.x, 2) <= pow(m_player.shape.radius + m_wall[i].length.x / 2, 2)
								&& pow(save + HEIGHT / m_line[stage].y * value - m_wall[i].pos.y, 2) <= pow(m_player.shape.radius + m_wall[i].length.y / 2, 2))
								m_player.flag[WALLCOLLISION] = true;
					}

					for (int i = 0; i < boxSize[stage]; i++) {
						if (pow(m_player.shape.pos.x - m_box[i].pos.x, 2) <= pow(m_player.shape.radius + m_box[i].length.y / 2, 2)
							&& pow(save + HEIGHT / m_line[stage].y * value - m_box[i].pos.y, 2) <= pow(m_player.shape.radius + m_box[i].length.y / 2, 2))
							m_player.flag[DRAW] = false;

						if (pow(m_player.shape.pos.x - m_box[i].pos.x, 2) <= pow(m_player.shape.radius + m_box[i].length.x / 2, 2)
							&& pow(m_player.shape.pos.y - m_box[i].pos.y, 2) <= pow(m_player.shape.radius + m_box[i].length.y / 2, 2)) {

							m_player.shape.pos.y = save;

							m_player.speed.x = m_box[i].speed.x;
							m_player.speed.y = m_box[i].speed.y;

							m_player.flag[BOXCOLLISION] = true;

							m_player.flag[COURSE] = true;

							value == OPPOSITION ? m_player.flag[INPUT] = false :
								m_player.flag[INPUT] = true;

							m_box[i].savePos.y = m_box[i].pos.y;

							m_box[i].flag[PLAYERCOLLISION] = true;
						}
					}

					if (!m_player.flag[BOXCOLLISION] && !m_player.flag[WALLCOLLISION]) {
						m_player.speed.x = m_player.saveSpeed.x;
						m_player.speed.y = m_player.saveSpeed.y;

						m_player.shape.pos.y += m_player.speed.y * value;

						if ((value == OPPOSITION && m_player.shape.pos.y <= save + HEIGHT / m_line[stage].y * value)
							|| (value == NORMAL && m_player.shape.pos.y >= save + HEIGHT / m_line[stage].y * value)) {
							m_player.score[STEP]++;
							m_player.score[WALK] += 10;

							m_player.shape.pos.y = save + HEIGHT / m_line[stage].y * value;
						}
					}


					if (m_player.flag[DRAW]) {
						SetDrawScreen(DX_SCREEN_BACK);
						Draw();
					}

					if (m_player.flag[BOXCOLLISION] || m_player.flag[WALLCOLLISION]) {
						m_player.flag[WALLCOLLISION] = false;

						m_player.flag[DRAW] = true;
						break;
					}
				}

			}
			else {
				while ((value == OPPOSITION && m_player.shape.pos.y > save + HEIGHT / m_line[stage].y * value)
					|| value == NORMAL && m_player.shape.pos.y < save + HEIGHT / m_line[stage].y * value) {
					for (int i = 0; i < boxSize[stage]; i++) {
						if (m_box[i].flag[PLAYERCOLLISION]) m_box[i].flag[COURSE] = true;

						if (value == OPPOSITION && m_box[i].savePos.y + HEIGHT / m_line[stage].y * value <= 0
							|| value == NORMAL && m_box[i].savePos.y + HEIGHT / m_line[stage].y * value >= HEIGHT)
							m_box[i].flag[MOVE] = false;

						if ((m_player.flag[INPUT] && m_player.shape.pos.y + m_player.speed.y * value < save)
							|| !m_player.flag[INPUT] && m_player.shape.pos.y + m_player.speed.y * value > save
							|| !m_player.flag[COURSE]) {
							m_player.flag[BOXCOLLISION] = false;

							for (int j = 0; j < boxSize[stage]; j++) {
								if (m_box[j].flag[PLAYERCOLLISION]) {
									m_box[j].flag[PLAYERCOLLISION] = false;
									m_box[i].flag[MOVE] = true;
								}
							}

							m_player.speed.x = m_player.saveSpeed.x;
							m_player.speed.y = m_player.saveSpeed.y;
						}

						if (m_box[i].flag[PLAYERCOLLISION] &&
							(value == OPPOSITION && m_box[i].pos.y - m_box[i].length.y / 2 <= 0
							|| value == NORMAL && m_box[i].pos.y + m_box[i].length.y / 2 >= HEIGHT)) {
							return;
						}

						if (m_box[i].flag[PLAYERCOLLISION] && m_wall_Save[i].flag) {
							if ((value == NORMAL && pow(m_box[i].pos.x - m_wall_Save[i].pos.x, 2) <= pow(m_box[i].length.x / 2 + m_wall_Save[i].length.x / 2, 2)
								&& pow(m_box[i].savePos.y + HEIGHT / m_line[stage].y * value + m_player.speed.y - m_wall_Save[i].pos.y, 2) <= pow(m_box[i].length.y / 2 + m_wall_Save[i].length.y / 2, 2))
								|| value == OPPOSITION && pow(m_box[i].pos.x - m_wall_Save[i].pos.x, 2) <= pow(m_box[i].length.x / 2 + m_wall_Save[i].length.x / 2, 2)
								&& pow(m_box[i].savePos.y - HEIGHT / m_line[stage].y * value - m_player.speed.y - m_wall_Save[i].pos.y, 2) >= pow(m_box[i].length.y / 2 + m_wall_Save[i].length.y / 2, 2)) {
								m_player.flag[WALLCOLLISION] = true;

								if (m_box[i].pos.x != m_wall_Save[i].pos.x && m_box[i].savePos.y + HEIGHT / m_line[stage].y * value + m_player.speed.y == m_wall_Save[i].pos.y
									|| m_box[i].pos.x != m_wall_Save[i].pos.x && m_box[i].savePos.y + HEIGHT / m_line[stage].y * value + m_player.speed.y != m_wall_Save[i].pos.y) {
									m_player.flag[WALLCOLLISION] = false;
									m_box[i].flag[MOVE] = true;

									m_wall_Save[i].flag = false;

									m_box[i].savePos.y = m_box[i].savePos.y + HEIGHT / m_line[stage].y * value;

									wallSaveFlag = false;
								}
							}
							else {
								m_player.flag[WALLCOLLISION] = false;
								m_box[i].flag[MOVE] = true;

								m_box[i].savePos.y = m_box[i].savePos.y + HEIGHT / m_line[stage].y * value;

								m_wall_Save[i].flag = false;

								wallSaveFlag = false;
							}
						}

						if (m_box[i].flag[PLAYERCOLLISION]) {
							for (int j = 0; j < boxSize[stage]; j++) {
								if (value == NORMAL) m_box[i].flag[INPUT] = true;
								else m_box[i].flag[INPUT] = false;

								if (i != j && (m_box[i].distance[BOX] == NULL
										|| m_box[i].distance[BOX] >= pow(m_box[i].pos.x - m_box[j].pos.x, 2) + pow(m_box[i].pos.y - m_box[j].pos.y, 2))
									&& !m_box[i].flag[BOX_BOXCOLLISION]) {
									if (pow(m_box[i].pos.x - m_box[j].pos.x, 2) <= pow(m_box[i].length.x / 2 + m_box[j].length.x / 2, 2)
										&& pow(m_box[i].pos.y - m_box[j].pos.y, 2) <= pow(m_box[i].length.y / 2 + m_box[j].length.y / 2, 2)) {
										m_box[i].flag[BOX_BOXCOLLISION] = true;

										if (m_box[i].pos.x != m_box[j].pos.x
											|| m_box[i].savePos.y + HEIGHT / m_line[stage].y * value != m_box[j].pos.y) {
											if (!m_box[i].flag[COURSE]
												|| value == NORMAL && !m_box[i].flag[INPUT] || value == OPPOSITION && m_box[i].flag[INPUT]
													|| m_box[i].pos.x - m_box[j].pos.x != 0 && m_box[i].pos.y - m_box[j].pos.y != 0
														|| m_box[i].pos.x - m_box[j].pos.x != 0 && m_box[i].pos.y - m_box[j].pos.y == 0) {
												m_box[i].flag[BOX_BOXCOLLISION] = false;
												m_box[i].flag[MOVE] = true;
											}
										}

										m_box[i].distance[BOX]
											= pow(m_box[i].pos.x - m_box[j].pos.x, 2) + pow(m_box[i].pos.y - m_box[j].pos.y, 2);
									}
									else {
										m_box[i].flag[BOX_BOXCOLLISION] = false;
										m_box[i].flag[MOVE] = true;
									}
								}
							}
						}

						for (int i = 0; i < boxSize[stage]; i++) {
							if (m_box[i].flag[BOX_BOXCOLLISION]) {
								m_box[i].flag[COURSE] = true;
								m_box[i].flag[MOVE] = true;

								if (value == NORMAL) m_box[i].flag[INPUT] = true;
								else m_box[i].flag[INPUT] = false;

								m_box[i].distance[BOX] = NULL;

								return;
							}
						}

						if (m_box[i].flag[PLAYERCOLLISION] && !m_box[i].flag[BOX_BOXCOLLISION] && !m_player.flag[WALLCOLLISION]) {
							if (m_player.flag[BOXCOLLISION]) {
								m_player.shape.pos.y += m_player.speed.y * value;

								if (m_player.flag[COURSE]) {
									m_box[i].pos.y += m_box[i].speed.y * value;

									if ((value == OPPOSITION && m_player.shape.pos.y <= save + HEIGHT / m_line[stage].y * value)
										|| value == NORMAL && m_player.shape.pos.y >= save + HEIGHT / m_line[stage].y * value) {
										m_player.shape.pos.y = save + HEIGHT / m_line[stage].y * value;

										m_player.score[STEP]++;
										m_player.score[WALK] += 10;

										if (wallSaveFlag) {
											m_box[i].pos.y = m_box[i].savePos.y + HEIGHT / m_line[stage].y * value;
											m_box[i].savePos.y = m_box[i].pos.y;
										}

										if (value == NORMAL) m_box[i].flag[INPUT] = true;
										else m_box[i].flag[INPUT] = false;

										m_box[i].distance[BOX] = NULL;
									}
								}
							}
						}

						m_box[i].flag[MOVE] = true;
					}

					if (!m_player.flag[BOXCOLLISION] || m_player.flag[WALLCOLLISION]) break;

					SetDrawScreen(DX_SCREEN_BACK);
					Draw();
				}
			}
		}
	}

	for (int i = 0; i < boxSize[stage] * wallSize[stage]; i++) {
		if (m_box[i / wallSize[stage]].flag[PLAYERCOLLISION] && m_wall[i % wallSize[stage]].flag) {
			if (!m_wall_Save[i / wallSize[stage]].flag && pow(m_box[i / wallSize[stage]].pos.x - m_wall[i % wallSize[stage]].pos.x, 2) <= pow(m_box[i / wallSize[stage]].length.x / 2 + m_wall[i % wallSize[stage]].length.x / 2, 2)
				&& pow(m_box[i / wallSize[stage]].pos.y - m_wall[i % wallSize[stage]].pos.y, 2) <= pow(m_box[i / wallSize[stage]].length.y / 2 + m_wall[i % wallSize[stage]].length.y / 2, 2)) {
				m_wall_Save[i / wallSize[stage]].pos.x = m_wall[i % wallSize[stage]].pos.x;
				m_wall_Save[i / wallSize[stage]].pos.y = m_wall[i % wallSize[stage]].pos.y;

				m_wall_Save[i / wallSize[stage]].length.x = m_wall[i % wallSize[stage]].length.x;
				m_wall_Save[i / wallSize[stage]].length.y = m_wall[i % wallSize[stage]].length.y;

				m_box[i / wallSize[stage]].distance[WALL] = pow(m_box[i / wallSize[stage]].pos.x - m_wall[i % wallSize[stage]].pos.x, 2) + pow(m_box[i / wallSize[stage]].pos.y - m_wall[i % wallSize[stage]].pos.y, 2);
				
				m_player.flag[WALLCOLLISION] = true;

				m_wall_Save[i / wallSize[stage]].flag = true;
			}
			else if (m_wall_Save[i / wallSize[stage]].flag
				&& pow(m_box[i / wallSize[stage]].pos.x - m_wall[i % wallSize[stage]].pos.x, 2) <= pow(m_box[i / wallSize[stage]].length.x / 2 + m_wall[i % wallSize[stage]].length.x / 2, 2)
				&& pow(m_box[i / wallSize[stage]].pos.y - m_wall[i % wallSize[stage]].pos.y, 2) <= pow(m_box[i / wallSize[stage]].length.y / 2 + m_wall[i % wallSize[stage]].length.y / 2, 2)
				&& m_box[i / wallSize[stage]].distance[WALL] > pow(m_box[i / wallSize[stage]].pos.x - m_wall[i % wallSize[stage]].pos.x, 2) + pow(m_box[i / wallSize[stage]].pos.y - m_wall[i % wallSize[stage]].pos.y, 2)) {
				m_wall_Save[i / wallSize[stage]].pos.x = m_wall[i % wallSize[stage]].pos.x;
				m_wall_Save[i / wallSize[stage]].pos.y = m_wall[i % wallSize[stage]].pos.y;

				m_wall_Save[i / wallSize[stage]].length.x = m_wall[i % wallSize[stage]].length.x;
				m_wall_Save[i / wallSize[stage]].length.y = m_wall[i % wallSize[stage]].length.y;

				m_box[i / wallSize[stage]].distance[WALL] = pow(m_box[i / wallSize[stage]].pos.x - m_wall[i % wallSize[stage]].pos.x, 2) + pow(m_box[i / wallSize[stage]].pos.y - m_wall[i % wallSize[stage]].pos.y, 2);
			}
		}
	}
}