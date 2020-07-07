#pragma once
class GameManager
{
public:
	// Width and Height of window in grid
	int N = 40, M = 40;

	// Size of image in pixels
	float img_size = 24;

	// Width of board in pixels
	float width_board = img_size * N;
	// Height of board in pixels
	float height_board = img_size * M;

	// Width of UI space in pixels
	float width_UI = img_size * 15;
	// Height of UI space in pixels
	float height_UI = height_board;

	void start();

	GameManager() {}
	~GameManager() {}
};

