#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>	
#include "DxLib.h"
#include "def.hpp"

void spritArea(int oldAreaIndex)
{
	int newAreaIndex = areaCount;
	int w = areas[oldAreaIndex].w;
	int h = areas[oldAreaIndex].h;

	if (rand() % 2 == 0)
	{
		areas[oldAreaIndex].w /= 2;
		areas[newAreaIndex].x = areas[oldAreaIndex].x + areas[oldAreaIndex].w;
		areas[newAreaIndex].y = areas[oldAreaIndex].y;
		areas[newAreaIndex].w = w - areas[oldAreaIndex].w;
		areas[newAreaIndex].h = areas[oldAreaIndex].h;
	}
	else
	{
		areas[oldAreaIndex].h /= 2;
		areas[newAreaIndex].x = areas[oldAreaIndex].x;
		areas[newAreaIndex].y = areas[oldAreaIndex].y + areas[oldAreaIndex].h;
		areas[newAreaIndex].w = areas[oldAreaIndex].w;
		areas[newAreaIndex].h = h - areas[oldAreaIndex].h;
	}
	//分割後の二つのエリアがエリアの条件を満たしているか
	if ((areas[oldAreaIndex].w < AREA_SIZE_MIN) || (areas[oldAreaIndex].h < AREA_SIZE_MIN)
		|| (areas[newAreaIndex].w < AREA_SIZE_MIN) || (areas[newAreaIndex].h < AREA_SIZE_MIN))
	{
		areas[oldAreaIndex].w = w;
		areas[oldAreaIndex].h = h;
		return;
	}

	areaCount++;

	spritArea(newAreaIndex);
	spritArea(oldAreaIndex);
}

void displayArea()
{
	int x, y, i;
	int field_num[FIELD_HEIGHT][FIELD_WIDTH] = { 0 };

	for (i = 0; i < areaCount; i++)
		for (y = areas[i].y; y < areas[i].y + areas[i].h; y++)
			for (x = areas[i].x; x < areas[i].x + areas[i].w; x++)
				field_num[y][x] = i;
	system("cls");
	for (y = 0; y < FIELD_HEIGHT; y++)
	{
		for (x = 0; x < FIELD_WIDTH; x++)
			DrawFormatString(x, y, BLACK, "%2d", field_num[y][x]);
		DrawFormatString(x, y, BLACK, "\n");
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
class Array2D
{
public:
	int Get(int x, int y);
	int Set(int x, int y, int v);
};
class Field
{
public:
	void Create(Array2D mapdata);
	int ToWorldX(int xgrid);
	int ToWorldY(int ygrid);
	int ToGridX(int xworld);
	int ToGridY(int yworld);
};

enum eDir {
	LEFT,
	UP,
	RIGHT,
	DOWN,
};