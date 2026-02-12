#define _CRT_SECURE_NO_WARNINGS
#include	<stdio.h>

int main() {
	FILE* fp;
	fp = fopen("map_data.txt", "w");
	if(fp == nullptr) printf("ファイルを開くことが出来ませんでした。")
}