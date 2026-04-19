#define _CRT_SECURE_NO_WARNINGS
#include	<stdio.h>
#include	<DxLib.h>

static int pos_Y = 0;

const int score[] = { 20,10,30,50,60 };
const int size = sizeof score / sizeof score[0];

static bool open = true;

static FILE* fp;

void fileWrite() {
	fp = fopen("data.txt", "w");

	if (fp != nullptr) {
		if (open) {
			for (int i = 0; i < size; i++) fprintf(fp, "%d\n", score[i]);
			open = false;
		}

		DrawString(0, pos_Y, "ファイルに保存しました。", 0xffffff);
	}

	if (CheckHitKey(KEY_INPUT_ESCAPE) || CheckHitKey(KEY_INPUT_O)) {
		fclose(fp);
		pos_Y += 20;
		open = true;
	}
}

void fileShow(FILE* file) {
	static int* save = new int[size];

	if (open) {
		for (int i = 0; i < size; i++) fscanf(file, "%d", &save[i]);
		open = false;
	}

	for (int i = 0; i < size; i++) DrawFormatString(0, 20 * i + pos_Y, 0xffffff, "%d", save[i]);

	if (CheckHitKey(KEY_INPUT_ESCAPE)) delete[] save;
}

void fileOpen() {
	fp = fopen("data.txt", "w");

	if(fp != nullptr) fileShow(fp);
	else DrawString(0, pos_Y, "ファイルを開くことが出来ませんでした。", 0xffffff);

	if (CheckHitKey(KEY_INPUT_ESCAPE) || CheckHitKey(KEY_INPUT_S)) {
		fclose(fp);
		pos_Y += 20;
		open = true;
	}
}