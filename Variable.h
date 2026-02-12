#pragma once
#define Vertical false
#define Horizontal true

const int WIDTH = 720;
const int HEIGHT = 480;
const int DIVISION = 25;

const int OPPOSITION = -1;
const int NORMAL = 1;

struct Vector2 {
	int x;
	int y;
};

struct Circle {
	Vector2 pos;
	int radius;
	int color;
};

struct Box {
	Vector2 pos;
	Vector2 length;
	int color;
	bool flag;
};