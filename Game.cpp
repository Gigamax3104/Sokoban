#include	"DxLib.h"
#include	"math.h"
#include	"Color.h"
#include	"Class.h"
#include	"Variable.h"

Game::Game() {
	playerSpeed = 1;
	savePlayerSpeed = playerSpeed;
	boxSpeed = 3;

	line.x = (int)sqrt(DIVISION);
	line.y = (int)sqrt(DIVISION);

	box = new Box[BOXSIZE];
	point = new Circle[POINTSIZE];

	player.pos.x = WIDTH / line.x / 2;
	player.pos.y = HEIGHT / line.y / 2;
	player.radius = 20;
	player.color = color[RED];

	for (int i = 0; i < BOXSIZE; i++) {
		box[i].pos.x = (WIDTH / line.x / 2) * (line.x - 1);
		box[i].pos.y = HEIGHT / 2;
		box[i].length.x = 50;
		box[i].length.y = 50;
		box[i].color = color[BLUE];
		box[i].flag = false;
	}

	for (int i = 0; i < POINTSIZE; i++) {
		point[i].pos.x = WIDTH / 2;
		point[i].pos.y = HEIGHT / 2;
		point[i].radius = 10;
		point[i].color = color[GREEN];
	}
}

void Game::Start() {
	Move();
	Draw();
}

void Game::Move() {
	if(CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_LEFT)) Judge(Horizontal, OPPOSITION);

	if(CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_RIGHT)) Judge(Horizontal, NORMAL);

	if(CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_UP)) Judge(Vertical, OPPOSITION);

	if(CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_DOWN)) Judge(Vertical, NORMAL);
}

void Game::Judge(bool direction, int value) {
	if (direction) { //左右を押された時
		//if ((pow(player.pos.x + playerSpeed * value - box.pos.x, 2) <= pow(player.radius + box.length.x / 2, 2)
		//	&& pow(player.pos.y - box.pos.y, 2) <= pow(player.radius + box.length.y / 2, 2)) //円と矩形の当たり判定(当たっている)
		//	&& (box.pos.x + boxSpeed * value > box.length.x / 2 && box.pos.x + boxSpeed * value < WIDTH - box.length.x / 2) //矩形のx軸の移動制限
		//	&& !box.flag) {  //ポイント部分にハマってない時
		int save = player.pos.x;
		
		while ((value == OPPOSITION && player.pos.x > save + (WIDTH / line.x) * value)
			|| (value == NORMAL && player.pos.x < save + (WIDTH / line.x) * value)) {
			player.pos.x += playerSpeed * value;

			if ((value == OPPOSITION && player.pos.x < save + (WIDTH / line.x) * value)
				|| (value == NORMAL && player.pos.x > save + (WIDTH / line.x) * value))
				player.pos.x = save + (WIDTH / line.x) * value;

			ClearDrawScreen();

			Draw();

			ScreenFlip();
		}

		//playerSpeed = boxSpeed;
		//player.pos.x += playerSpeed * value;
		//box.pos.x += boxSpeed * value;

		//if (pow(box.pos.x - point.pos.x,2) + pow(box.pos.y - point.pos.y,2) <= pow(point.radius,2)) {
		//	box.pos = point.pos;
		//	player.pos.x = box.pos.x + box.length.x / 2 * value;
		//	box.color = color[LIGHTBLUE];
		//	box.flag = true;
		//}

		//playerSpeed = savePlayerSpeed;

	//}
//	if (pow(player.pos.x + playerSpeed * value - box.pos.x, 2) <= pow(player.radius + box.length.x / 2, 2)
//		&& pow(player.pos.y - box.pos.y, 2) <= pow(player.radius + box.length.y / 2, 2) //円と矩形の当たり判定(当たっている)
//		&& (box.pos.x + boxSpeed * value > box.length.x / 2 && box.pos.x + boxSpeed * value < WIDTH - box.length.x / 2)//矩形のx軸の移動制限
//		&& box.flag){ //ポイント部分にハマっている時
//		playerSpeed = boxSpeed;
//		player.pos.x += playerSpeed * value;
//		box.pos.x += boxSpeed * value;

//		box.color = color[BLUE];
//		playerSpeed = savePlayerSpeed;
//	}
//	else if (player.pos.x + playerSpeed * value > player.radius && player.pos.x + playerSpeed * value < WIDTH - player.radius //円のx軸の移動制限
//		&& !(pow(player.pos.x + playerSpeed * value - box.pos.x, 2) <= pow(player.radius + box.length.x / 2, 2)
//			&& pow(player.pos.y - box.pos.y, 2) <= pow(player.radius + box.length.y / 2, 2))) { //円と矩形の当たり判定(当たっていない)
//		box.flag = false;
//		player.pos.x += playerSpeed * value;
//	}
	}
	else { //上下を押された時
		int save = player.pos.y;

		while ((value == OPPOSITION && player.pos.y > save + (HEIGHT / line.y) * value)
			|| (value == NORMAL && player.pos.y < save + (HEIGHT / line.y) * value)) {

			player.pos.y += playerSpeed * value;
			if ((value == OPPOSITION && player.pos.y < save + (HEIGHT / line.y) * value)
				|| (value == NORMAL && player.pos.y > save + (HEIGHT / line.y) * value))
				player.pos.y = save + (HEIGHT / line.y) * value;

			ClearDrawScreen();

			Draw();

			ScreenFlip();
		}

//	if ((pow(player.pos.x - box.pos.x, 2) <= pow(player.radius + box.length.x / 2, 2)
//		&& pow(player.pos.y + playerSpeed * value - box.pos.y, 2) <= pow(player.radius + box.length.y / 2, 2)) //円と矩形の当たり判定(当たっている)
//		&& (box.pos.y + boxSpeed * value > box.length.y / 2 && box.pos.y + boxSpeed * value < HEIGHT - box.length.y / 2) //矩形のy軸の移動制限
//		&& !box.flag) {
//		playerSpeed = boxSpeed;
//		player.pos.y += playerSpeed * value;
//		box.pos.y += boxSpeed * value;

//		if (pow(box.pos.x - point.pos.x, 2) + pow(box.pos.y - point.pos.y, 2) <= pow(point.radius, 2)) {
//			box.pos = point.pos;
//			box.color = color[LIGHTBLUE];
//			box.flag = true;
//		}

//		playerSpeed = savePlayerSpeed;

//	}
//	else if ((pow(player.pos.x - box.pos.x, 2) <= pow(player.radius + box.length.x / 2, 2)
//		&& pow(player.pos.y + playerSpeed * value - box.pos.y, 2) <= pow(player.radius + box.length.y / 2, 2)) //円と矩形の当たり判定(当たっている)
//		&& (box.pos.y + boxSpeed * value > box.length.y / 2 && box.pos.y + boxSpeed * value < HEIGHT - box.length.y / 2) //矩形のy軸の移動制限
//		&& box.flag) {
//			playerSpeed = boxSpeed;
//			player.pos.y += playerSpeed * value;
//			box.pos.y += boxSpeed * value;

//			box.color = color[BLUE];
//			playerSpeed = savePlayerSpeed;
//		 }
//	else if (player.pos.y + playerSpeed * value > player.radius && player.pos.y + playerSpeed * value < HEIGHT - player.radius //円のy軸の移動制限
//		&& !(pow(player.pos.x - box.pos.x, 2) <= pow(player.radius + box.length.x / 2, 2)
//			&& pow(player.pos.y + playerSpeed * value - box.pos.y, 2) <= pow(player.radius + box.length.y / 2, 2))) { //円と矩形の当たり判定(当たっていない)
//		box.flag = false;
//		player.pos.y += playerSpeed * value;
//	}
	}

}

void Game::Draw() {
	for (int i = 0; i < line.x; i++) if (i != 0) DrawLine(0, i * (HEIGHT / line.y), WIDTH, i * (HEIGHT / line.y), color[WHITE]);
	for (int i = 0; i < line.y; i++) if (i != 0) DrawLine(i * (WIDTH / line.x), 0, i * (WIDTH / line.x), HEIGHT, color[WHITE]);

	//DrawFormatString(0, 0, color[WHITE], "%d", (int)(pow(point.radius,2)));
	//DrawFormatString(0, 20, color[WHITE], "%d", (int)(pow(box.pos.x - point.pos.x, 2) + pow(box.pos.y - point.pos.y, 2)));
	DrawFormatString(0, 0, color[WHITE], "%d", WIDTH / line.x / 2);
	for (int i = 0; i < POINTSIZE; i++) DrawCircle(point[i].pos.x, point[i].pos.y, point[i].radius, point[i].color, TRUE);

	DrawCircle(player.pos.x, player.pos.y, player.radius, player.color, TRUE);

	for (int i = 0; i < BOXSIZE; i++) DrawBox(box[i].pos.x - box[i].length.x / 2, box[i].pos.y - box[i].length.y / 2,
		box[i].pos.x + box[i].length.x / 2, box[i].pos.y + box[i].length.y / 2, box[i].color, TRUE);
}

Game::~Game() {
	delete[] box;
	box = nullptr;

	delete[] point;
	point = nullptr;
}
